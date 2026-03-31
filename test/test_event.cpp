/* Unit tests for vwr::event_handler and vwr::delegate
 * Copyright (c) 2026 Veritaware
 * SPDX-License-Identifier: Zlib
 */

#include <CWEL/event.hpp>

#include <catch2/catch_test_macros.hpp>

namespace
{
struct test_event_args
{
    int value;
};

int g_call_count_a = 0;
int g_call_count_b = 0;
int g_call_count_c = 0;
int g_sum = 0;
const void* g_last_sender = nullptr;

void reset_state()
{
    g_call_count_a = 0;
    g_call_count_b = 0;
    g_call_count_c = 0;
    g_sum = 0;
    g_last_sender = nullptr;
}

void callback_a(const void* sender, const test_event_args& args)
{
    ++g_call_count_a;
    g_sum += args.value;
    g_last_sender = sender;
}

void callback_b(const void* sender, const test_event_args& args)
{
    ++g_call_count_b;
    g_sum += args.value * 10;
    g_last_sender = sender;
}

void callback_c(const void* sender, const test_event_args& args)
{
    ++g_call_count_c;
    g_sum += args.value * 100;
    g_last_sender = sender;
}

void null_safe_callback(const void*, const test_event_args&) {}
} // namespace

TEST_CASE("event - subscribe and invoke single delegate", "[event][invoke]")
{
    reset_state();

    vwr::event_handler<test_event_args> handler;
    vwr::delegate d(callback_a);

    handler += d;

    constexpr int sender = 123;
    handler(&sender, test_event_args{5});

    REQUIRE(g_call_count_a == 1);
    REQUIRE(g_call_count_b == 0);
    REQUIRE(g_sum == 5);
    REQUIRE(g_last_sender == &sender);
}

TEST_CASE("event - multiple delegates are invoked", "[event][invoke]")
{
    reset_state();

    vwr::event_handler<test_event_args> handler;
    vwr::delegate d1(callback_a);
    vwr::delegate d2(callback_b);

    handler += d1;
    handler += d2;

    handler(nullptr, test_event_args{2});

    REQUIRE(g_call_count_a == 1);
    REQUIRE(g_call_count_b == 1);
    REQUIRE(g_sum == 22);
}

TEST_CASE("event - adding the same delegate twice does not duplicate subscription", "[event][subscribe]")
{
    reset_state();

    vwr::event_handler<test_event_args> handler;
    vwr::delegate d(callback_a);

    handler += d;
    handler += d;

    handler(nullptr, test_event_args{3});

    REQUIRE(g_call_count_a == 1);
    REQUIRE(g_sum == 3);
}

TEST_CASE("event - operator-= unsubscribes delegate", "[event][unsubscribe]")
{
    reset_state();

    vwr::event_handler<test_event_args> handler;
    vwr::delegate d(callback_a);

    handler += d;
    handler -= d;

    handler(nullptr, test_event_args{7});

    REQUIRE(g_call_count_a == 0);
    REQUIRE(g_sum == 0);
}

TEST_CASE("event - delegate destructor auto-unsubscribes", "[event][lifetime]")
{
    reset_state();

    vwr::event_handler<test_event_args> handler;
    {
        vwr::delegate d(callback_a);
        handler += d;
    }

    handler(nullptr, test_event_args{9});

    REQUIRE(g_call_count_a == 0);
    REQUIRE(g_sum == 0);
}

TEST_CASE("event - handler destruction allows delegate to outlive handler safely", "[event][lifetime]")
{
    reset_state();

    auto* d = new vwr::delegate(callback_a);
    {
        vwr::event_handler<test_event_args> handler;
        handler += *d;
        handler(nullptr, test_event_args{1});
        REQUIRE(g_call_count_a == 1);
    }

    delete d;
    SUCCEED();
}

TEST_CASE("event - delegate can be reused after previous handler is destroyed", "[event][lifetime]")
{
    reset_state();

    vwr::delegate d(callback_a);

    {
        vwr::event_handler<test_event_args> handler_a;
        handler_a += d;
        handler_a(nullptr, test_event_args{1});
    }

    vwr::event_handler<test_event_args> handler_b;
    handler_b += d;
    handler_b(nullptr, test_event_args{4});

    REQUIRE(g_call_count_a == 2);
    REQUIRE(g_sum == 5);
}

TEST_CASE("event - null callback is safe", "[event][invoke]")
{
    reset_state();

    vwr::event_handler<test_event_args> handler;
    vwr::delegate<test_event_args> d(nullptr);
    vwr::delegate d2(null_safe_callback);

    handler += d;
    handler += d2;

    handler(nullptr, test_event_args{11});

    REQUIRE(g_call_count_a == 0);
    REQUIRE(g_call_count_b == 0);
    REQUIRE(g_sum == 0);
}

TEST_CASE("event - delegate removed between emits is not called again", "[event][edge-case][unsubscribe]")
{
    reset_state();

    vwr::event_handler<test_event_args> handler;
    vwr::delegate self(callback_a);
    vwr::delegate other(callback_b);

    handler += self;
    handler += other;

    handler(nullptr, test_event_args{2});

    REQUIRE(g_call_count_a == 1);
    REQUIRE(g_call_count_b == 1);
    REQUIRE(g_sum == 22);

    handler -= self;
    handler(nullptr, test_event_args{3});

    REQUIRE(g_call_count_a == 1);
    REQUIRE(g_call_count_b == 2);
    REQUIRE(g_sum == 52);
}

TEST_CASE("event - removing one delegate between emits leaves remaining delegates active", "[event][edge-case][unsubscribe]")
{
    reset_state();

    vwr::event_handler<test_event_args> handler;
    vwr::delegate remover(callback_a);
    vwr::delegate removed(callback_b);

    handler += remover;
    handler += removed;

    handler(nullptr, test_event_args{2});

    REQUIRE(g_call_count_a == 1);
    REQUIRE(g_call_count_b == 1);
    REQUIRE(g_sum == 22);

    handler -= removed;
    handler(nullptr, test_event_args{4});

    REQUIRE(g_call_count_a == 2);
    REQUIRE(g_call_count_b == 1);
    REQUIRE(g_sum == 26);
}

TEST_CASE("event - adding a delegate between emits affects only subsequent emits", "[event][edge-case][subscribe]")
{
    reset_state();

    vwr::event_handler<test_event_args> handler;
    vwr::delegate installer(callback_a);
    vwr::delegate late(callback_c);

    handler += installer;

    handler(nullptr, test_event_args{2});

    REQUIRE(g_call_count_a == 1);
    REQUIRE(g_call_count_c == 0);
    REQUIRE(g_sum == 2);

    handler += late;
    handler(nullptr, test_event_args{3});

    REQUIRE(g_call_count_a == 2);
    REQUIRE(g_call_count_c == 1);
    REQUIRE(g_sum == 305);
}

