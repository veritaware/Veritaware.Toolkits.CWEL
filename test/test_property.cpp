/* Unit tests for vwr::Property<T>
 * Copyright (c) 2026 Veritaware
 * SPDX-License-Identifier: Zlib
 */

#include <CWEL/property.hpp>

#include <catch2/catch_test_macros.hpp>

#include <string>

namespace
{
int gPropertyChangedCount = 0;
int gLastOldValue = 0;
int gLastNewValue = 0;
const void* gLastSender = nullptr;

void resetPropertyState()
{
    gPropertyChangedCount = 0;
    gLastOldValue = 0;
    gLastNewValue = 0;
    gLastSender = nullptr;
}

void onIntPropertyChanged(const void* sender, const vwr::PropertyChangedEventArgs<int>& args)
{
    ++gPropertyChangedCount;
    gLastOldValue = args.OldValue;
    gLastNewValue = args.NewValue;
    gLastSender = sender;
}
} // namespace

TEST_CASE("Property - Construction and value access", "[Property][Constructor]")
{
    SECTION("Default construction works for default-constructible class types")
    {
        vwr::Property<std::string> value;
        REQUIRE(value.Get().empty());
        REQUIRE(value().empty());
    }

    SECTION("Construction with initial value stores that value")
    {
        vwr::Property<int> value(42);
        REQUIRE(value.Get() == 42);
        REQUIRE(value() == 42);
    }
}

TEST_CASE("Property - PropertyChangedEventArgs stores old and new value", "[Property][EventArgs]")
{
    vwr::PropertyChangedEventArgs<int> args(3, 7);

    REQUIRE(args.OldValue == 3);
    REQUIRE(args.NewValue == 7);
}

TEST_CASE("Property - Set updates value and raises PropertyChanged event", "[Property][Set][Event]")
{
    resetPropertyState();

    vwr::Property<int> value(10);
    vwr::Delegate changedDelegate(onIntPropertyChanged);
    value.PropertyChanged += changedDelegate;

    value.Set(25);

    REQUIRE(value.Get() == 25);
    REQUIRE(gPropertyChangedCount == 1);
    REQUIRE(gLastOldValue == 10);
    REQUIRE(gLastNewValue == 25);
    REQUIRE(gLastSender == &value);
}

TEST_CASE("Property - operator= from raw value updates value and raises event", "[Property][Assignment][Event]")
{
    resetPropertyState();

    vwr::Property<int> value(5);
    vwr::Delegate changedDelegate(onIntPropertyChanged);
    value.PropertyChanged += changedDelegate;

    value = 9;

    REQUIRE(value.Get() == 9);
    REQUIRE(gPropertyChangedCount == 1);
    REQUIRE(gLastOldValue == 5);
    REQUIRE(gLastNewValue == 9);
    REQUIRE(gLastSender == &value);
}

TEST_CASE("Property - Assigning the same value does not raise event", "[Property][Set][Event]")
{
    resetPropertyState();

    vwr::Property<int> value(12);
    vwr::Delegate changedDelegate(onIntPropertyChanged);
    value.PropertyChanged += changedDelegate;

    value.Set(12);

    REQUIRE(value.Get() == 12);
    REQUIRE(gPropertyChangedCount == 0);
}

TEST_CASE("Property - Copy construction copies value but not subscriptions", "[Property][Copy]")
{
    resetPropertyState();

    vwr::Property<int> original(21);
    vwr::Delegate originalDelegate(onIntPropertyChanged);
    original.PropertyChanged += originalDelegate;

    vwr::Property<int> copy(original);

    REQUIRE(copy.Get() == 21);

    copy.Set(30);

    REQUIRE(copy.Get() == 30);
    REQUIRE(gPropertyChangedCount == 0);

    original.Set(40);

    REQUIRE(gPropertyChangedCount == 1);
    REQUIRE(gLastOldValue == 21);
    REQUIRE(gLastNewValue == 40);
    REQUIRE(gLastSender == &original);
}

TEST_CASE("Property - Copied Property can have its own subscriptions independent of source", "[Property][Copy][Event]")
{
    resetPropertyState();

    vwr::Property<int> original(1);
    vwr::Property<int> copy(original);
    vwr::Delegate copyDelegate(onIntPropertyChanged);
    copy.PropertyChanged += copyDelegate;

    copy.Set(8);

    REQUIRE(gPropertyChangedCount == 1);
    REQUIRE(gLastOldValue == 1);
    REQUIRE(gLastNewValue == 8);
    REQUIRE(gLastSender == &copy);
}

TEST_CASE("Property - Assignment from another Property uses source value and raises event only on change", "[Property][Copy][Assignment]")
{
    SECTION("Assignment from Property with different value raises event")
    {
        resetPropertyState();

        vwr::Property<int> source(33);
        vwr::Property<int> destination(11);
        vwr::Delegate destinationDelegate(onIntPropertyChanged);
        destination.PropertyChanged += destinationDelegate;

        destination = source;

        REQUIRE(destination.Get() == 33);
        REQUIRE(gPropertyChangedCount == 1);
        REQUIRE(gLastOldValue == 11);
        REQUIRE(gLastNewValue == 33);
        REQUIRE(gLastSender == &destination);
    }

    SECTION("Assignment from Property with equal value does not raise event")
    {
        resetPropertyState();

        vwr::Property<int> source(44);
        vwr::Property<int> destination(44);
        vwr::Delegate destinationDelegate(onIntPropertyChanged);
        destination.PropertyChanged += destinationDelegate;

        destination = source;

        REQUIRE(destination.Get() == 44);
        REQUIRE(gPropertyChangedCount == 0);
    }
}

