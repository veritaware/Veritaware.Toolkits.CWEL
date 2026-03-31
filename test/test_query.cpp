/* Unit tests for vwr::query<T>
 * Copyright (c) 2026 Veritaware
 * SPDX-License-Identifier: Zlib
 */

#include <CWEL/query.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

// ---------------------------------------------------------------------------
// Constructors and get()
// ---------------------------------------------------------------------------
TEST_CASE("query - construction and get", "[query][constructor]")
{
    SECTION("construct from vector")
    {
        std::vector<int> v = {1, 2, 3};
        vwr::query<int> q(v);
        REQUIRE(q.get() == v);
    }

    SECTION("construct from initializer list")
    {
        vwr::query<int> q{10, 20, 30};
        REQUIRE(q.get() == std::vector<int>{10, 20, 30});
    }

    SECTION("construct from iterator range")
    {
        std::vector<int> v = {4, 5, 6};
        vwr::query<int> q(v.begin(), v.end());
        REQUIRE(q.get() == v);
    }

    SECTION("construct from empty vector")
    {
        vwr::query<int> q(std::vector<int>{});
        REQUIRE(q.get().empty());
    }

    SECTION("get returns a copy, not a reference")
    {
        vwr::query<int> q{1, 2, 3};
        auto copy = q.get();
        copy.push_back(4);
        REQUIRE(q.get().size() == 3);
    }
}

// ---------------------------------------------------------------------------
// aggregate
// ---------------------------------------------------------------------------

TEST_CASE("query - aggregate", "[query][aggregate]")
{
    SECTION("aggregate with seed")
    {
        vwr::query<int> q{1, 2, 3, 4};
        int result = q.aggregate(0, [](int acc, int v) { return acc + v; });
        REQUIRE(result == 10);
    }

    SECTION("aggregate with seed uses seed as initial value")
    {
        vwr::query<int> q{1, 2, 3};
        int result = q.aggregate(10, [](int acc, int v) { return acc + v; });
        REQUIRE(result == 16);
    }

    SECTION("aggregate without seed uses first element")
    {
        vwr::query<int> q{1, 2, 3, 4};
        int result = q.aggregate([](int acc, int v) { return acc + v; });
        REQUIRE(result == 10);
    }

    SECTION("aggregate without seed on single element returns that element")
    {
        vwr::query<int> q{42};
        REQUIRE(q.aggregate([](int acc, int v) { return acc + v; }) == 42);
    }

    SECTION("aggregate without seed throws on empty sequence")
    {
        vwr::query<int> q(std::vector<int>{});
        REQUIRE_THROWS_AS(q.aggregate([](int acc, int v) { return acc + v; }), std::out_of_range);
    }

    SECTION("aggregate with seed on empty sequence returns seed")
    {
        vwr::query<int> q(std::vector<int>{});
        REQUIRE(q.aggregate(99, [](int acc, int v) { return acc + v; }) == 99);
    }
}

// ---------------------------------------------------------------------------
// first / first_or_default
// ---------------------------------------------------------------------------

TEST_CASE("query - first", "[query][first]")
{
    SECTION("first returns the first element")
    {
        vwr::query<int> q{10, 20, 30};
        REQUIRE(q.first() == 10);
    }

    SECTION("first throws on empty sequence")
    {
        vwr::query<int> q(std::vector<int>{});
        REQUIRE_THROWS_AS(q.first(), std::out_of_range);
    }

    SECTION("first on const query returns first element")
    {
        const vwr::query<int> q{5, 6, 7};
        REQUIRE(q.first() == 5);
    }

    SECTION("first_or_default returns first element when non-empty")
    {
        vwr::query<int> q{3, 1, 4};
        REQUIRE(q.first_or_default() == 3);
    }

    SECTION("first_or_default returns default when empty")
    {
        vwr::query<int> q(std::vector<int>{});
        REQUIRE(q.first_or_default() == 0);
    }

    SECTION("first_or_default returns default-constructed value for strings")
    {
        vwr::query<std::string> q(std::vector<std::string>{});
        REQUIRE(q.first_or_default().empty());
    }
}

// ---------------------------------------------------------------------------
// last / last_or_default
// ---------------------------------------------------------------------------

TEST_CASE("query - last", "[query][last]")
{
    SECTION("last returns the last element")
    {
        vwr::query<int> q{10, 20, 30};
        REQUIRE(q.last() == 30);
    }

    SECTION("last throws on empty sequence")
    {
        vwr::query<int> q(std::vector<int>{});
        REQUIRE_THROWS_AS(q.last(), std::out_of_range);
    }

    SECTION("last on const query returns last element")
    {
        const vwr::query<int> q{5, 6, 7};
        REQUIRE(q.last() == 7);
    }

    SECTION("last_or_default returns last element when non-empty")
    {
        vwr::query<int> q{3, 1, 4};
        REQUIRE(q.last_or_default() == 4);
    }

    SECTION("last_or_default returns default when empty")
    {
        vwr::query<int> q(std::vector<int>{});
        REQUIRE(q.last_or_default() == 0);
    }
}

// ---------------------------------------------------------------------------
// single / single_or_default
// ---------------------------------------------------------------------------

TEST_CASE("query - single", "[query][single]")
{
    SECTION("single returns the only element")
    {
        vwr::query<int> q{42};
        REQUIRE(q.single() == 42);
    }

    SECTION("single throws on empty sequence")
    {
        vwr::query<int> q(std::vector<int>{});
        REQUIRE_THROWS_AS(q.single(), std::out_of_range);
    }

    SECTION("single throws when more than one element")
    {
        vwr::query<int> q{1, 2};
        REQUIRE_THROWS_AS(q.single(), std::out_of_range);
    }

    SECTION("single on const query")
    {
        const vwr::query<int> q{7};
        REQUIRE(q.single() == 7);
    }

    SECTION("single_or_default returns the only element")
    {
        vwr::query<int> q{99};
        REQUIRE(q.single_or_default() == 99);
    }

    SECTION("single_or_default returns default on empty sequence")
    {
        vwr::query<int> q(std::vector<int>{});
        REQUIRE(q.single_or_default() == 0);
    }

    SECTION("single_or_default throws when more than one element")
    {
        vwr::query<int> q{1, 2};
        REQUIRE_THROWS_AS(q.single_or_default(), std::out_of_range);
    }
}

// ---------------------------------------------------------------------------
// cast
// ---------------------------------------------------------------------------

TEST_CASE("query - cast", "[query][cast]")
{
    SECTION("cast int to double")
    {
        vwr::query<int> q{1, 2, 3};
        auto result = q.cast<double>().get();
        REQUIRE(result == std::vector<double>{1.0, 2.0, 3.0});
    }

    SECTION("cast int to float")
    {
        vwr::query<int> q{4, 5};
        auto result = q.cast<float>().get();
        REQUIRE(result == std::vector<float>{4.0F, 5.0F});
    }

    SECTION("cast to same type produces equal sequence")
    {
        vwr::query<int> q{7, 8, 9};
        REQUIRE(q.cast<int>().get() == std::vector<int>{7, 8, 9});
    }

    SECTION("cast on empty sequence returns empty query")
    {
        vwr::query<int> q(std::vector<int>{});
        REQUIRE(q.cast<double>().get().empty());
    }
}

// ---------------------------------------------------------------------------
// concat
// ---------------------------------------------------------------------------

TEST_CASE("query - concat", "[query][concat]")
{
    SECTION("concat joins two non-empty sequences")
    {
        vwr::query<int> a{1, 2, 3};
        vwr::query<int> b{4, 5, 6};
        REQUIRE(a.concat(b).get() == std::vector<int>{1, 2, 3, 4, 5, 6});
    }

    SECTION("concat with empty right sequence returns left")
    {
        vwr::query<int> a{1, 2};
        vwr::query<int> b(std::vector<int>{});
        REQUIRE(a.concat(b).get() == std::vector<int>{1, 2});
    }

    SECTION("concat with empty left sequence returns right")
    {
        vwr::query<int> a(std::vector<int>{});
        vwr::query<int> b{3, 4};
        REQUIRE(a.concat(b).get() == std::vector<int>{3, 4});
    }

    SECTION("concat preserves duplicates")
    {
        vwr::query<int> a{1, 2};
        vwr::query<int> b{2, 3};
        REQUIRE(a.concat(b).get() == std::vector<int>{1, 2, 2, 3});
    }
}

// ---------------------------------------------------------------------------
// distinct
// ---------------------------------------------------------------------------

TEST_CASE("query - distinct", "[query][distinct]")
{
    SECTION("distinct removes duplicate ints")
    {
        vwr::query<int> q{1, 2, 2, 3, 1};
        REQUIRE(q.distinct().get() == std::vector<int>{1, 2, 3});
    }

    SECTION("distinct on already-unique sequence returns same sequence")
    {
        vwr::query<int> q{4, 5, 6};
        REQUIRE(q.distinct().get() == std::vector<int>{4, 5, 6});
    }

    SECTION("distinct on empty sequence returns empty")
    {
        vwr::query<int> q(std::vector<int>{});
        REQUIRE(q.distinct().get().empty());
    }

    SECTION("distinct with custom comparer (case-insensitive strings)")
    {
        vwr::query<std::string> q{"hello", "HELLO", "world"};
        auto ci = [](const std::string& a, const std::string& b)
        {
            if(a.size() != b.size())
                return false;
            for(size_t i = 0; i < a.size(); ++i)
                if(std::tolower(static_cast<unsigned char>(a[i])) != std::tolower(static_cast<unsigned char>(b[i])))
                    return false;
            return true;
        };
        auto result = q.distinct(ci).get();
        REQUIRE(result == std::vector<std::string>{"hello", "world"});
    }
}

// ---------------------------------------------------------------------------
// except
// ---------------------------------------------------------------------------

TEST_CASE("query - except", "[query][except]")
{
    SECTION("except removes elements present in the other sequence")
    {
        vwr::query<int> a{1, 2, 3, 4};
        vwr::query<int> b{2, 4};
        REQUIRE(a.except(b).get() == std::vector<int>{1, 3});
    }

    SECTION("except with no overlap returns all unique elements of left")
    {
        vwr::query<int> a{1, 2, 3};
        vwr::query<int> b{4, 5};
        REQUIRE(a.except(b).get() == std::vector<int>{1, 2, 3});
    }

    SECTION("except with full overlap returns empty")
    {
        vwr::query<int> a{1, 2};
        vwr::query<int> b{1, 2, 3};
        REQUIRE(a.except(b).get().empty());
    }

    SECTION("except deduplicates the result")
    {
        vwr::query<int> a{1, 1, 2, 3};
        vwr::query<int> b{3};
        REQUIRE(a.except(b).get() == std::vector<int>{1, 2});
    }

    SECTION("except on empty left sequence returns empty")
    {
        vwr::query<int> a(std::vector<int>{});
        vwr::query<int> b{1, 2};
        REQUIRE(a.except(b).get().empty());
    }

    SECTION("except with custom comparer")
    {
        vwr::query<int> a{1, 2, 3, 4};
        vwr::query<int> b{3, 4, 5};
        auto eq = [](int x, int y) { return x == y; };
        REQUIRE(a.except(b, eq).get() == std::vector<int>{1, 2});
    }
}

// ---------------------------------------------------------------------------
// intersect
// ---------------------------------------------------------------------------

TEST_CASE("query - intersect", "[query][intersect]")
{
    SECTION("intersect returns common elements")
    {
        vwr::query<int> a{1, 2, 3, 4};
        vwr::query<int> b{2, 4, 6};
        REQUIRE(a.intersect(b).get() == std::vector<int>{2, 4});
    }

    SECTION("intersect with no overlap returns empty")
    {
        vwr::query<int> a{1, 2};
        vwr::query<int> b{3, 4};
        REQUIRE(a.intersect(b).get().empty());
    }

    SECTION("intersect deduplicates the result")
    {
        vwr::query<int> a{1, 1, 2};
        vwr::query<int> b{1};
        REQUIRE(a.intersect(b).get() == std::vector<int>{1});
    }

    SECTION("intersect on empty left returns empty")
    {
        vwr::query<int> a(std::vector<int>{});
        vwr::query<int> b{1, 2};
        REQUIRE(a.intersect(b).get().empty());
    }

    SECTION("intersect with custom comparer")
    {
        vwr::query<int> a{1, 2, 3};
        vwr::query<int> b{2, 3, 4};
        auto eq = [](int x, int y) { return x == y; };
        REQUIRE(a.intersect(b, eq).get() == std::vector<int>{2, 3});
    }
}

// ---------------------------------------------------------------------------
// join
// ---------------------------------------------------------------------------

TEST_CASE("query - join", "[query][join]")
{
    SECTION("join correlates matching keys")
    {
        vwr::query<int> outer{1, 2, 3};
        vwr::query<int> inner{2, 3, 4};
        auto result = outer.join(
                                inner,
                                [](int x) { return x; },
                                [](int y) { return y; })
                          .get();
        REQUIRE(result == (std::vector<std::pair<int, int>>{{2, 2}, {3, 3}}));
    }

    SECTION("join with no matching keys returns empty")
    {
        vwr::query<int> outer{1, 2};
        vwr::query<int> inner{3, 4};
        auto result = outer.join(
                                inner,
                                [](int x) { return x; },
                                [](int y) { return y; })
                          .get();
        REQUIRE(result.empty());
    }

    SECTION("join produces a cross-product for repeated keys")
    {
        vwr::query<int> outer{1, 1};
        vwr::query<int> inner{1};
        auto result = outer.join(
                                inner,
                                [](int x) { return x; },
                                [](int y) { return y; })
                          .get();
        REQUIRE(result.size() == 2);
    }

    SECTION("join with custom comparer")
    {
        vwr::query<int> outer{2, 4};
        vwr::query<int> inner{1, 3};
        // match when outer value is one more than inner value
        auto eq = [](int inner_key, int outer_key) { return outer_key - inner_key == 1; };
        auto result = outer.join(
                                inner,
                                [](int x) { return x; },
                                [](int y) { return y; },
                                eq)
                          .get();
        REQUIRE(result == (std::vector<std::pair<int, int>>{{2, 1}, {4, 3}}));
    }
}

// ---------------------------------------------------------------------------
// reverse
// ---------------------------------------------------------------------------

TEST_CASE("query - reverse", "[query][reverse]")
{
    SECTION("reverse inverts the sequence")
    {
        vwr::query<int> q{1, 2, 3, 4, 5};
        REQUIRE(q.reverse().get() == std::vector<int>{5, 4, 3, 2, 1});
    }

    SECTION("reverse on single element returns same")
    {
        vwr::query<int> q{42};
        REQUIRE(q.reverse().get() == std::vector<int>{42});
    }

    SECTION("reverse on empty returns empty")
    {
        vwr::query<int> q(std::vector<int>{});
        REQUIRE(q.reverse().get().empty());
    }

    SECTION("double reverse returns original sequence")
    {
        vwr::query<int> q{1, 2, 3};
        REQUIRE(q.reverse().reverse().get() == std::vector<int>{1, 2, 3});
    }
}

// ---------------------------------------------------------------------------
// sequence_equal
// ---------------------------------------------------------------------------

TEST_CASE("query - sequence_equal", "[query][sequence_equal]")
{
    SECTION("equal sequences")
    {
        vwr::query<int> a{1, 2, 3};
        vwr::query<int> b{1, 2, 3};
        REQUIRE(a.sequence_equal(b));
    }

    SECTION("different sizes are not equal")
    {
        vwr::query<int> a{1, 2};
        vwr::query<int> b{1, 2, 3};
        REQUIRE_FALSE(a.sequence_equal(b));
    }

    SECTION("same size but different elements")
    {
        vwr::query<int> a{1, 2, 3};
        vwr::query<int> b{1, 2, 4};
        REQUIRE_FALSE(a.sequence_equal(b));
    }

    SECTION("empty sequences are equal")
    {
        vwr::query<int> a(std::vector<int>{});
        vwr::query<int> b(std::vector<int>{});
        REQUIRE(a.sequence_equal(b));
    }

    SECTION("sequence_equal with custom comparer")
    {
        vwr::query<std::string> a{"Hello", "World"};
        vwr::query<std::string> b{"hello", "world"};
        auto ci = [](const std::string& x, const std::string& y)
        {
            if(x.size() != y.size())
                return false;
            for(size_t i = 0; i < x.size(); ++i)
                if(std::tolower(static_cast<unsigned char>(x[i])) != std::tolower(static_cast<unsigned char>(y[i])))
                    return false;
            return true;
        };
        REQUIRE(a.sequence_equal(b, ci));
    }
}

// ---------------------------------------------------------------------------
// skip_while
// ---------------------------------------------------------------------------

TEST_CASE("query - skip_while", "[query][skip_while]")
{
    SECTION("skips elements while predicate is true")
    {
        vwr::query<int> q{1, 2, 3, 4, 5};
        auto result = q.skip_while([](int v) { return v < 3; }).get();
        REQUIRE(result == std::vector<int>{3, 4, 5});
    }

    SECTION("skips nothing when predicate is false from the start")
    {
        vwr::query<int> q{5, 4, 3};
        auto result = q.skip_while([](int v) { return v < 3; }).get();
        REQUIRE(result == std::vector<int>{5, 4, 3});
    }

    SECTION("skips all when predicate is always true")
    {
        vwr::query<int> q{1, 2, 3};
        auto result = q.skip_while([](int) { return true; }).get();
        REQUIRE(result.empty());
    }

    SECTION("skip_while does not skip elements after first false")
    {
        vwr::query<int> q{1, 2, 1, 3};
        auto result = q.skip_while([](int v) { return v < 2; }).get();
        REQUIRE(result == std::vector<int>{2, 1, 3});
    }
}

// ---------------------------------------------------------------------------
// take_while
// ---------------------------------------------------------------------------

TEST_CASE("query - take_while", "[query][take_while]")
{
    SECTION("takes elements while predicate is true")
    {
        vwr::query<int> q{1, 2, 3, 4, 5};
        auto result = q.take_while([](int v) { return v < 4; }).get();
        REQUIRE(result == std::vector<int>{1, 2, 3});
    }

    SECTION("takes nothing when predicate is false from the start")
    {
        vwr::query<int> q{5, 4, 3};
        auto result = q.take_while([](int v) { return v < 3; }).get();
        REQUIRE(result.empty());
    }

    SECTION("takes all when predicate is always true")
    {
        vwr::query<int> q{1, 2, 3};
        auto result = q.take_while([](int) { return true; }).get();
        REQUIRE(result == std::vector<int>{1, 2, 3});
    }

    SECTION("take_while stops at first false even if later elements pass")
    {
        vwr::query<int> q{1, 2, 5, 1, 2};
        auto result = q.take_while([](int v) { return v < 4; }).get();
        REQUIRE(result == std::vector<int>{1, 2});
    }
}

// ---------------------------------------------------------------------------
// unite
// ---------------------------------------------------------------------------

TEST_CASE("query - unite", "[query][unite]")
{
    SECTION("unite merges two sequences keeping only unique elements")
    {
        vwr::query<int> a{1, 2, 3};
        vwr::query<int> b{2, 3, 4};
        REQUIRE(a.unite(b).get() == std::vector<int>{1, 2, 3, 4});
    }

    SECTION("unite with no overlap returns all elements")
    {
        vwr::query<int> a{1, 2};
        vwr::query<int> b{3, 4};
        REQUIRE(a.unite(b).get() == std::vector<int>{1, 2, 3, 4});
    }

    SECTION("unite with identical sequences returns unique elements")
    {
        vwr::query<int> a{1, 2, 3};
        vwr::query<int> b{1, 2, 3};
        REQUIRE(a.unite(b).get() == std::vector<int>{1, 2, 3});
    }

    SECTION("unite with empty sequence returns non-empty unique")
    {
        vwr::query<int> a{1, 1, 2};
        vwr::query<int> b(std::vector<int>{});
        REQUIRE(a.unite(b).get() == std::vector<int>{1, 2});
    }
}

// ---------------------------------------------------------------------------
// where
// ---------------------------------------------------------------------------

TEST_CASE("query - where", "[query][where]")
{
    SECTION("where filters elements by predicate")
    {
        vwr::query<int> q{1, 2, 3, 4, 5, 6};
        auto result = q.where([](int v) { return v % 2 == 0; }).get();
        REQUIRE(result == std::vector<int>{2, 4, 6});
    }

    SECTION("where returns empty when no element satisfies predicate")
    {
        vwr::query<int> q{1, 3, 5};
        auto result = q.where([](int v) { return v % 2 == 0; }).get();
        REQUIRE(result.empty());
    }

    SECTION("where returns all when all elements satisfy predicate")
    {
        vwr::query<int> q{2, 4, 6};
        auto result = q.where([](int v) { return v % 2 == 0; }).get();
        REQUIRE(result == std::vector<int>{2, 4, 6});
    }

    SECTION("where on empty sequence returns empty")
    {
        vwr::query<int> q(std::vector<int>{});
        REQUIRE(q.where([](int) { return true; }).get().empty());
    }
}

// ---------------------------------------------------------------------------
// all / any
// ---------------------------------------------------------------------------

TEST_CASE("query - all", "[query][all]")
{
    SECTION("all returns true when all elements satisfy predicate")
    {
        vwr::query<int> q{2, 4, 6};
        REQUIRE(q.all([](int v) { return v % 2 == 0; }));
    }

    SECTION("all returns false when at least one element does not satisfy")
    {
        vwr::query<int> q{2, 3, 6};
        REQUIRE_FALSE(q.all([](int v) { return v % 2 == 0; }));
    }

    SECTION("all on empty sequence returns true (vacuous truth)")
    {
        vwr::query<int> q(std::vector<int>{});
        REQUIRE(q.all([](int) { return false; }));
    }
}

TEST_CASE("query - any", "[query][any]")
{
    SECTION("any returns true when at least one element satisfies predicate")
    {
        vwr::query<int> q{1, 3, 4};
        REQUIRE(q.any([](int v) { return v % 2 == 0; }));
    }

    SECTION("any returns false when no element satisfies predicate")
    {
        vwr::query<int> q{1, 3, 5};
        REQUIRE_FALSE(q.any([](int v) { return v % 2 == 0; }));
    }

    SECTION("any on empty sequence returns false")
    {
        vwr::query<int> q(std::vector<int>{});
        REQUIRE_FALSE(q.any([](int) { return true; }));
    }
}

// ---------------------------------------------------------------------------
// average
// ---------------------------------------------------------------------------

TEST_CASE("query - average", "[query][average]")
{
    SECTION("average of integers returns double")
    {
        vwr::query<int> q{1, 2, 3, 4, 5};
        REQUIRE_THAT(q.average(), Catch::Matchers::WithinRel(3.0, 1e-9));
    }

    SECTION("average of floats")
    {
        vwr::query<float> q{1.0F, 2.0F, 3.0F};
        REQUIRE_THAT(q.average(), Catch::Matchers::WithinRel(2.0F, 1e-5F));
    }

    SECTION("average of doubles")
    {
        vwr::query<double> q{1.5, 2.5, 3.0};
        REQUIRE_THAT(q.average(), Catch::Matchers::WithinRel(7.0 / 3.0, 1e-9));
    }

    SECTION("average throws on empty sequence")
    {
        vwr::query<int> q(std::vector<int>{});
        REQUIRE_THROWS_AS(q.average(), std::out_of_range);
    }

    SECTION("average of single element")
    {
        vwr::query<int> q{7};
        REQUIRE_THAT(q.average(), Catch::Matchers::WithinRel(7.0, 1e-9));
    }
}

// ---------------------------------------------------------------------------
// contains
// ---------------------------------------------------------------------------

TEST_CASE("query - contains", "[query][contains]")
{
    SECTION("contains returns true for present element")
    {
        vwr::query<int> q{1, 2, 3};
        REQUIRE(q.contains(2));
    }

    SECTION("contains returns false for absent element")
    {
        vwr::query<int> q{1, 2, 3};
        REQUIRE_FALSE(q.contains(5));
    }

    SECTION("contains on empty sequence returns false")
    {
        vwr::query<int> q(std::vector<int>{});
        REQUIRE_FALSE(q.contains(1));
    }

    SECTION("contains works with strings")
    {
        vwr::query<std::string> q{"foo", "bar", "baz"};
        REQUIRE(q.contains("bar"));
        REQUIRE_FALSE(q.contains("qux"));
    }
}

// ---------------------------------------------------------------------------
// max / min
// ---------------------------------------------------------------------------

TEST_CASE("query - max", "[query][max]")
{
    SECTION("max returns maximum element")
    {
        vwr::query<int> q{3, 1, 4, 1, 5, 9, 2, 6};
        REQUIRE(q.max() == 9);
    }

    SECTION("max on single element returns that element")
    {
        vwr::query<int> q{42};
        REQUIRE(q.max() == 42);
    }

    SECTION("max throws on empty sequence")
    {
        vwr::query<int> q(std::vector<int>{});
        REQUIRE_THROWS_AS(q.max(), std::out_of_range);
    }

    SECTION("max with negative values")
    {
        vwr::query<int> q{-5, -3, -1, -4};
        REQUIRE(q.max() == -1);
    }
}

TEST_CASE("query - min", "[query][min]")
{
    SECTION("min returns minimum element")
    {
        vwr::query<int> q{3, 1, 4, 1, 5, 9, 2, 6};
        REQUIRE(q.min() == 1);
    }

    SECTION("min on single element returns that element")
    {
        vwr::query<int> q{7};
        REQUIRE(q.min() == 7);
    }

    SECTION("min throws on empty sequence")
    {
        vwr::query<int> q(std::vector<int>{});
        REQUIRE_THROWS_AS(q.min(), std::out_of_range);
    }

    SECTION("min with negative values")
    {
        vwr::query<int> q{-5, -3, -1, -4};
        REQUIRE(q.min() == -5);
    }
}

// ---------------------------------------------------------------------------
// sum
// ---------------------------------------------------------------------------

TEST_CASE("query - sum", "[query][sum]")
{
    SECTION("sum of integers")
    {
        vwr::query<int> q{1, 2, 3, 4, 5};
        REQUIRE(q.sum() == 15);
    }

    SECTION("sum of doubles")
    {
        vwr::query<double> q{1.1, 2.2, 3.3};
        REQUIRE_THAT(q.sum(), Catch::Matchers::WithinRel(6.6, 1e-9));
    }

    SECTION("sum throws on empty sequence")
    {
        vwr::query<int> q(std::vector<int>{});
        REQUIRE_THROWS_AS(q.sum(), std::out_of_range);
    }

    SECTION("sum of single element")
    {
        vwr::query<int> q{99};
        REQUIRE(q.sum() == 99);
    }

    SECTION("sum with negative values")
    {
        vwr::query<int> q{-1, -2, 3};
        REQUIRE(q.sum() == 0);
    }
}

// ---------------------------------------------------------------------------
// Chaining
// ---------------------------------------------------------------------------

TEST_CASE("query - method chaining", "[query][chaining]")
{
    SECTION("where then sum")
    {
        vwr::query<int> q{1, 2, 3, 4, 5, 6};
        REQUIRE(q.where([](int v) { return v % 2 == 0; }).sum() == 12);
    }

    SECTION("where then max")
    {
        vwr::query<int> q{1, 5, 2, 8, 3};
        REQUIRE(q.where([](int v) { return v < 6; }).max() == 5);
    }

    SECTION("distinct then reverse then get")
    {
        vwr::query<int> q{3, 1, 2, 1, 3};
        auto result = q.distinct().reverse().get();
        REQUIRE(result == std::vector<int>{2, 1, 3});
    }

    SECTION("concat then where then count via get")
    {
        vwr::query<int> a{1, 2, 3};
        vwr::query<int> b{4, 5, 6};
        auto result = a.concat(b).where([](int v) { return v > 3; }).get();
        REQUIRE(result == std::vector<int>{4, 5, 6});
    }
}
