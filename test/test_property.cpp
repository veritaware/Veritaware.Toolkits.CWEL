/* Unit tests for vwr::property<T>
 * Copyright (c) 2026 Veritaware
 * SPDX-License-Identifier: Zlib
 */

#include <CWEL/property.hpp>

#include <catch2/catch_test_macros.hpp>

#include <string>

namespace
{
int g_property_changed_count = 0;
int g_last_old_value = 0;
int g_last_new_value = 0;
const void* g_last_sender = nullptr;

void reset_property_state()
{
    g_property_changed_count = 0;
    g_last_old_value = 0;
    g_last_new_value = 0;
    g_last_sender = nullptr;
}

void on_int_property_changed(const void* sender, const vwr::property_changed_event_args<int>& args)
{
    ++g_property_changed_count;
    g_last_old_value = args.old_value;
    g_last_new_value = args.new_value;
    g_last_sender = sender;
}
} // namespace

TEST_CASE("property - construction and value access", "[property][constructor]")
{
    SECTION("default construction works for default-constructible class types")
    {
        vwr::property<std::string> value;
        REQUIRE(value.get().empty());
        REQUIRE(value().empty());
    }

    SECTION("construction with initial value stores that value")
    {
        vwr::property<int> value(42);
        REQUIRE(value.get() == 42);
        REQUIRE(value() == 42);
    }
}

TEST_CASE("property - property_changed_event_args stores old and new value", "[property][event-args]")
{
    vwr::property_changed_event_args<int> args(3, 7);

    REQUIRE(args.old_value == 3);
    REQUIRE(args.new_value == 7);
}

TEST_CASE("property - set updates value and raises property_changed event", "[property][set][event]")
{
    reset_property_state();

    vwr::property<int> value(10);
    vwr::delegate changed_delegate(on_int_property_changed);
    value.property_changed += changed_delegate;

    value.set(25);

    REQUIRE(value.get() == 25);
    REQUIRE(g_property_changed_count == 1);
    REQUIRE(g_last_old_value == 10);
    REQUIRE(g_last_new_value == 25);
    REQUIRE(g_last_sender == &value);
}

TEST_CASE("property - operator= from raw value updates value and raises event", "[property][assignment][event]")
{
    reset_property_state();

    vwr::property<int> value(5);
    vwr::delegate changed_delegate(on_int_property_changed);
    value.property_changed += changed_delegate;

    value = 9;

    REQUIRE(value.get() == 9);
    REQUIRE(g_property_changed_count == 1);
    REQUIRE(g_last_old_value == 5);
    REQUIRE(g_last_new_value == 9);
    REQUIRE(g_last_sender == &value);
}

TEST_CASE("property - assigning the same value does not raise event", "[property][set][event]")
{
    reset_property_state();

    vwr::property<int> value(12);
    vwr::delegate changed_delegate(on_int_property_changed);
    value.property_changed += changed_delegate;

    value.set(12);

    REQUIRE(value.get() == 12);
    REQUIRE(g_property_changed_count == 0);
}

TEST_CASE("property - copy construction copies value but not subscriptions", "[property][copy]")
{
    reset_property_state();

    vwr::property<int> original(21);
    vwr::delegate original_delegate(on_int_property_changed);
    original.property_changed += original_delegate;

    vwr::property<int> copy(original);

    REQUIRE(copy.get() == 21);

    copy.set(30);

    REQUIRE(copy.get() == 30);
    REQUIRE(g_property_changed_count == 0);

    original.set(40);

    REQUIRE(g_property_changed_count == 1);
    REQUIRE(g_last_old_value == 21);
    REQUIRE(g_last_new_value == 40);
    REQUIRE(g_last_sender == &original);
}

TEST_CASE("property - copied property can have its own subscriptions independent of source", "[property][copy][event]")
{
    reset_property_state();

    vwr::property<int> original(1);
    vwr::property<int> copy(original);
    vwr::delegate copy_delegate(on_int_property_changed);
    copy.property_changed += copy_delegate;

    copy.set(8);

    REQUIRE(g_property_changed_count == 1);
    REQUIRE(g_last_old_value == 1);
    REQUIRE(g_last_new_value == 8);
    REQUIRE(g_last_sender == &copy);
}

TEST_CASE("property - assignment from another property uses source value and raises event only on change", "[property][copy][assignment]")
{
    SECTION("assignment from property with different value raises event")
    {
        reset_property_state();

        vwr::property<int> source(33);
        vwr::property<int> destination(11);
        vwr::delegate destination_delegate(on_int_property_changed);
        destination.property_changed += destination_delegate;

        destination = source;

        REQUIRE(destination.get() == 33);
        REQUIRE(g_property_changed_count == 1);
        REQUIRE(g_last_old_value == 11);
        REQUIRE(g_last_new_value == 33);
        REQUIRE(g_last_sender == &destination);
    }

    SECTION("assignment from property with equal value does not raise event")
    {
        reset_property_state();

        vwr::property<int> source(44);
        vwr::property<int> destination(44);
        vwr::delegate destination_delegate(on_int_property_changed);
        destination.property_changed += destination_delegate;

        destination = source;

        REQUIRE(destination.get() == 44);
        REQUIRE(g_property_changed_count == 0);
    }
}

