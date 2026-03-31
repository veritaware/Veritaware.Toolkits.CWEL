/* Unit tests for vwr::EventHandler and vwr::Delegate
 * Copyright (c) 2026 Veritaware
 * SPDX-License-Identifier: Zlib
 */

#include <CWEL/event.hpp>

#include <catch2/catch_test_macros.hpp>

namespace
{
struct TestEventArgs
{
    int value;
};

int gCallCountA = 0;
int gCallCountB = 0;
int gCallCountC = 0;
int gSum = 0;
const void* gLastSender = nullptr;

void resetState()
{
    gCallCountA = 0;
    gCallCountB = 0;
    gCallCountC = 0;
    gSum = 0;
    gLastSender = nullptr;
}

void callbackA(const void* sender, const TestEventArgs& args)
{
    ++gCallCountA;
    gSum += args.value;
    gLastSender = sender;
}

void callbackB(const void* sender, const TestEventArgs& args)
{
    ++gCallCountB;
    gSum += args.value * 10;
    gLastSender = sender;
}

void callbackC(const void* sender, const TestEventArgs& args)
{
    ++gCallCountC;
    gSum += args.value * 100;
    gLastSender = sender;
}

void nullSafeCallback(const void*, const TestEventArgs&) {}
} // namespace

TEST_CASE("Event - Subscribe and invoke single Delegate", "[Event][Invoke]")
{
    resetState();

    vwr::EventHandler<TestEventArgs> handler;
    vwr::Delegate d(callbackA);

    handler += d;

    constexpr int sender = 123;
    handler(&sender, TestEventArgs{5});

    REQUIRE(gCallCountA == 1);
    REQUIRE(gCallCountB == 0);
    REQUIRE(gSum == 5);
    REQUIRE(gLastSender == &sender);
}

TEST_CASE("Event - Multiple Delegates are invoked", "[Event][Invoke]")
{
    resetState();

    vwr::EventHandler<TestEventArgs> handler;
    vwr::Delegate d1(callbackA);
    vwr::Delegate d2(callbackB);

    handler += d1;
    handler += d2;

    handler(nullptr, TestEventArgs{2});

    REQUIRE(gCallCountA == 1);
    REQUIRE(gCallCountB == 1);
    REQUIRE(gSum == 22);
}

TEST_CASE("Event - Adding the same Delegate twice does not duplicate subscription", "[Event][Subscribe]")
{
    resetState();

    vwr::EventHandler<TestEventArgs> handler;
    vwr::Delegate d(callbackA);

    handler += d;
    handler += d;

    handler(nullptr, TestEventArgs{3});

    REQUIRE(gCallCountA == 1);
    REQUIRE(gSum == 3);
}

TEST_CASE("Event - operator-= unsubscribes Delegate", "[Event][Unsubscribe]")
{
    resetState();

    vwr::EventHandler<TestEventArgs> handler;
    vwr::Delegate d(callbackA);

    handler += d;
    handler -= d;

    handler(nullptr, TestEventArgs{7});

    REQUIRE(gCallCountA == 0);
    REQUIRE(gSum == 0);
}

TEST_CASE("Event - Delegate destructor auto-unsubscribes", "[Event][Lifetime]")
{
    resetState();

    vwr::EventHandler<TestEventArgs> handler;
    {
        vwr::Delegate d(callbackA);
        handler += d;
    }

    handler(nullptr, TestEventArgs{9});

    REQUIRE(gCallCountA == 0);
    REQUIRE(gSum == 0);
}

TEST_CASE("Event - EventHandler destruction allows Delegate to outlive handler safely", "[Event][Lifetime]")
{
    resetState();

    auto* d = new vwr::Delegate(callbackA);
    {
        vwr::EventHandler<TestEventArgs> handler;
        handler += *d;
        handler(nullptr, TestEventArgs{1});
        REQUIRE(gCallCountA == 1);
    }

    delete d;
    SUCCEED();
}

TEST_CASE("Event - Delegate can be reused after previous EventHandler is destroyed", "[Event][Lifetime]")
{
    resetState();

    vwr::Delegate d(callbackA);

    {
        vwr::EventHandler<TestEventArgs> handlerA;
        handlerA += d;
        handlerA(nullptr, TestEventArgs{1});
    }

    vwr::EventHandler<TestEventArgs> handlerB;
    handlerB += d;
    handlerB(nullptr, TestEventArgs{4});

    REQUIRE(gCallCountA == 2);
    REQUIRE(gSum == 5);
}

TEST_CASE("Event - Null callback is safe", "[Event][Invoke]")
{
    resetState();

    vwr::EventHandler<TestEventArgs> handler;
    vwr::Delegate<TestEventArgs> d(nullptr);
    vwr::Delegate d2(nullSafeCallback);

    handler += d;
    handler += d2;

    handler(nullptr, TestEventArgs{11});

    REQUIRE(gCallCountA == 0);
    REQUIRE(gCallCountB == 0);
    REQUIRE(gSum == 0);
}

TEST_CASE("Event - Delegate removed between emits is not called again", "[Event][EdgeCase][Unsubscribe]")
{
    resetState();

    vwr::EventHandler<TestEventArgs> handler;
    vwr::Delegate self(callbackA);
    vwr::Delegate other(callbackB);

    handler += self;
    handler += other;

    handler(nullptr, TestEventArgs{2});

    REQUIRE(gCallCountA == 1);
    REQUIRE(gCallCountB == 1);
    REQUIRE(gSum == 22);

    handler -= self;
    handler(nullptr, TestEventArgs{3});

    REQUIRE(gCallCountA == 1);
    REQUIRE(gCallCountB == 2);
    REQUIRE(gSum == 52);
}

TEST_CASE("Event - Removing one Delegate between emits leaves remaining Delegates active", "[Event][EdgeCase][Unsubscribe]")
{
    resetState();

    vwr::EventHandler<TestEventArgs> handler;
    vwr::Delegate remover(callbackA);
    vwr::Delegate removed(callbackB);

    handler += remover;
    handler += removed;

    handler(nullptr, TestEventArgs{2});

    REQUIRE(gCallCountA == 1);
    REQUIRE(gCallCountB == 1);
    REQUIRE(gSum == 22);

    handler -= removed;
    handler(nullptr, TestEventArgs{4});

    REQUIRE(gCallCountA == 2);
    REQUIRE(gCallCountB == 1);
    REQUIRE(gSum == 26);
}

TEST_CASE("Event - Adding a Delegate between emits affects only subsequent emits", "[Event][EdgeCase][Subscribe]")
{
    resetState();

    vwr::EventHandler<TestEventArgs> handler;
    vwr::Delegate installer(callbackA);
    vwr::Delegate late(callbackC);

    handler += installer;

    handler(nullptr, TestEventArgs{2});

    REQUIRE(gCallCountA == 1);
    REQUIRE(gCallCountC == 0);
    REQUIRE(gSum == 2);

    handler += late;
    handler(nullptr, TestEventArgs{3});

    REQUIRE(gCallCountA == 2);
    REQUIRE(gCallCountC == 1);
    REQUIRE(gSum == 305);
}

