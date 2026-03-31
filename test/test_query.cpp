/* Unit tests for vwr::Query<T>
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
// Constructors and Get()
// ---------------------------------------------------------------------------
TEST_CASE("Query - Construction and Get", "[Query][Constructor]")
{
    SECTION("Construct from vector")
    {
        std::vector v = {1, 2, 3};
        vwr::Query q(v);
        REQUIRE(q.Get() == v);
    }

    SECTION("Construct from initializer list")
    {
        vwr::Query q{10, 20, 30};
        REQUIRE(q.Get() == std::vector<int>{10, 20, 30});
    }

    SECTION("Construct from iterator range")
    {
        std::vector v = {4, 5, 6};
        vwr::Query<int> q(v.begin(), v.end());
        REQUIRE(q.Get() == v);
    }

    SECTION("Construct from empty vector")
    {
        vwr::Query q(std::vector<int>{});
        REQUIRE(q.Get().empty());
    }

        SECTION("Get returns a copy, not a reference")
    {
        vwr::Query q{1, 2, 3};
        auto copy = q.Get();
        copy.push_back(4);
        REQUIRE(q.Get().size() == 3);
    }
}

// ---------------------------------------------------------------------------
// Aggregate
// ---------------------------------------------------------------------------

TEST_CASE("Query - Aggregate", "[Query][Aggregate]")
{
    SECTION("Aggregate with seed")
    {
        vwr::Query q{1, 2, 3, 4};
        int result = q.Aggregate(0, [](const int acc, const int v) { return acc + v; });
        REQUIRE(result == 10);
    }

    SECTION("Aggregate with seed uses seed as initial value")
    {
        vwr::Query q{1, 2, 3};
        int result = q.Aggregate(10, [](const int acc, const int v) { return acc + v; });
        REQUIRE(result == 16);
    }

    SECTION("Aggregate without seed uses first element")
    {
        vwr::Query q{1, 2, 3, 4};
        int result = q.Aggregate([](const int acc, const int v) { return acc + v; });
        REQUIRE(result == 10);
    }

    SECTION("Aggregate without seed on single element returns that element")
    {
        vwr::Query q{42};
        REQUIRE(q.Aggregate([](int acc, int v) { return acc + v; }) == 42);
    }

    SECTION("Aggregate without seed throws on empty sequence")
    {
        vwr::Query q(std::vector<int>{});
        REQUIRE_THROWS_AS(q.Aggregate([](const int acc, const int v) { return acc + v; }), std::out_of_range);
    }

    SECTION("Aggregate with seed on empty sequence returns seed")
    {
        vwr::Query q(std::vector<int>{});
        REQUIRE(q.Aggregate(99, [](int acc, int v) { return acc + v; }) == 99);
    }
}

// ---------------------------------------------------------------------------
// First / FirstOrDefault
// ---------------------------------------------------------------------------

TEST_CASE("Query - First", "[Query][First]")
{
    SECTION("First returns the first element")
    {
        vwr::Query q{10, 20, 30};
        REQUIRE(q.First() == 10);
    }

    SECTION("First throws on empty sequence")
    {
        vwr::Query q(std::vector<int>{});
        REQUIRE_THROWS_AS(q.First(), std::out_of_range);
    }

    SECTION("First on const Query returns first element")
    {
        const vwr::Query q{5, 6, 7};
        REQUIRE(q.First() == 5);
    }

    SECTION("FirstOrDefault returns first element when non-empty")
    {
        vwr::Query q{3, 1, 4};
        REQUIRE(q.FirstOrDefault() == 3);
    }

    SECTION("FirstOrDefault returns default when empty")
    {
        vwr::Query q(std::vector<int>{});
        REQUIRE(q.FirstOrDefault() == 0);
    }

    SECTION("FirstOrDefault returns default-constructed value for strings")
    {
        vwr::Query q(std::vector<std::string>{});
        REQUIRE(q.FirstOrDefault().empty());
    }
}

// ---------------------------------------------------------------------------
// Last / LastOrDefault
// ---------------------------------------------------------------------------

TEST_CASE("Query - Last", "[Query][Last]")
{
    SECTION("Last returns the last element")
    {
        vwr::Query q{10, 20, 30};
        REQUIRE(q.Last() == 30);
    }

    SECTION("Last throws on empty sequence")
    {
        vwr::Query q(std::vector<int>{});
        REQUIRE_THROWS_AS(q.Last(), std::out_of_range);
    }

    SECTION("Last on const Query returns last element")
    {
        const vwr::Query q{5, 6, 7};
        REQUIRE(q.Last() == 7);
    }

    SECTION("LastOrDefault returns last element when non-empty")
    {
        vwr::Query q{3, 1, 4};
        REQUIRE(q.LastOrDefault() == 4);
    }

    SECTION("LastOrDefault returns default when empty")
    {
        vwr::Query q(std::vector<int>{});
        REQUIRE(q.LastOrDefault() == 0);
    }
}

// ---------------------------------------------------------------------------
// Single / SingleOrDefault
// ---------------------------------------------------------------------------

TEST_CASE("Query - Single", "[Query][Single]")
{
    SECTION("Single returns the only element")
    {
        vwr::Query q{42};
        REQUIRE(q.Single() == 42);
    }

    SECTION("Single throws on empty sequence")
    {
        vwr::Query q(std::vector<int>{});
        REQUIRE_THROWS_AS(q.Single(), std::out_of_range);
    }

    SECTION("Single throws when more than one element")
    {
        vwr::Query q{1, 2};
        REQUIRE_THROWS_AS(q.Single(), std::out_of_range);
    }

    SECTION("Single on const Query")
    {
        const vwr::Query q{7};
        REQUIRE(q.Single() == 7);
    }

    SECTION("SingleOrDefault returns the only element")
    {
        vwr::Query q{99};
        REQUIRE(q.SingleOrDefault() == 99);
    }

    SECTION("SingleOrDefault returns default on empty sequence")
    {
        vwr::Query q(std::vector<int>{});
        REQUIRE(q.SingleOrDefault() == 0);
    }

    SECTION("SingleOrDefault throws when more than one element")
    {
        vwr::Query q{1, 2};
        REQUIRE_THROWS_AS(q.SingleOrDefault(), std::out_of_range);
    }
}

// ---------------------------------------------------------------------------
// Cast
// ---------------------------------------------------------------------------

TEST_CASE("Query - Cast", "[Query][Cast]")
{
    SECTION("Cast int to double")
    {
        vwr::Query q{1, 2, 3};
        auto result = q.Cast<double>().Get();
        REQUIRE(result == std::vector<double>{1.0, 2.0, 3.0});
    }

    SECTION("Cast int to float")
    {
        vwr::Query q{4, 5};
        auto result = q.Cast<float>().Get();
        REQUIRE(result == std::vector<float>{4.0F, 5.0F});
    }

    SECTION("Cast to same type produces equal sequence")
    {
        vwr::Query q{7, 8, 9};
        REQUIRE(q.Cast<int>().Get() == std::vector<int>{7, 8, 9});
    }

    SECTION("Cast on empty sequence returns empty Query")
    {
        vwr::Query q(std::vector<int>{});
        REQUIRE(q.Cast<double>().Get().empty());
    }
}

// ---------------------------------------------------------------------------
// Concat
// ---------------------------------------------------------------------------

TEST_CASE("Query - Concat", "[Query][Concat]")
{
    SECTION("Concat joins two non-empty sequences")
    {
        vwr::Query a{1, 2, 3};
        vwr::Query b{4, 5, 6};
        REQUIRE(a.Concat(b).Get() == std::vector<int>{1, 2, 3, 4, 5, 6});
    }

    SECTION("Concat with empty right sequence returns left")
    {
        vwr::Query a{1, 2};
        vwr::Query b(std::vector<int>{});
        REQUIRE(a.Concat(b).Get() == std::vector<int>{1, 2});
    }

    SECTION("Concat with empty left sequence returns right")
    {
        vwr::Query a(std::vector<int>{});
        vwr::Query b{3, 4};
        REQUIRE(a.Concat(b).Get() == std::vector<int>{3, 4});
    }

    SECTION("Concat preserves duplicates")
    {
        vwr::Query a{1, 2};
        vwr::Query b{2, 3};
        REQUIRE(a.Concat(b).Get() == std::vector<int>{1, 2, 2, 3});
    }
}

// ---------------------------------------------------------------------------
// Distinct
// ---------------------------------------------------------------------------

TEST_CASE("Query - Distinct", "[Query][Distinct]")
{
    SECTION("Distinct removes duplicate ints")
    {
        vwr::Query q{1, 2, 2, 3, 1};
        REQUIRE(q.Distinct().Get() == std::vector<int>{1, 2, 3});
    }

    SECTION("Distinct on already-unique sequence returns same sequence")
    {
        vwr::Query q{4, 5, 6};
        REQUIRE(q.Distinct().Get() == std::vector<int>{4, 5, 6});
    }

    SECTION("Distinct on empty sequence returns empty")
    {
        vwr::Query q(std::vector<int>{});
        REQUIRE(q.Distinct().Get().empty());
    }

    SECTION("Distinct with custom comparer (case-insensitive strings)")
    {
        vwr::Query<std::string> q{"hello", "HELLO", "world"};
        auto ci = [](const std::string& a, const std::string& b)
        {
            if(a.size() != b.size())
                return false;
            for(size_t i = 0; i < a.size(); ++i)
                if(std::tolower(static_cast<unsigned char>(a[i])) != std::tolower(static_cast<unsigned char>(b[i])))
                    return false;
            return true;
        };
        auto result = q.Distinct(ci).Get();
        REQUIRE(result == std::vector<std::string>{"hello", "world"});
    }
}

// ---------------------------------------------------------------------------
// Except
// ---------------------------------------------------------------------------

TEST_CASE("Query - Except", "[Query][Except]")
{
    SECTION("Except removes elements present in the other sequence")
    {
        vwr::Query a{1, 2, 3, 4};
        vwr::Query b{2, 4};
        REQUIRE(a.Except(b).Get() == std::vector<int>{1, 3});
    }

    SECTION("Except with no overlap returns all unique elements of left")
    {
        vwr::Query a{1, 2, 3};
        vwr::Query b{4, 5};
        REQUIRE(a.Except(b).Get() == std::vector<int>{1, 2, 3});
    }

    SECTION("Except with full overlap returns empty")
    {
        vwr::Query a{1, 2};
        vwr::Query b{1, 2, 3};
        REQUIRE(a.Except(b).Get().empty());
    }

    SECTION("Except deduplicates the result")
    {
        vwr::Query a{1, 1, 2, 3};
        vwr::Query b{3};
        REQUIRE(a.Except(b).Get() == std::vector<int>{1, 2});
    }

    SECTION("Except on empty left sequence returns empty")
    {
        vwr::Query a(std::vector<int>{});
        vwr::Query b{1, 2};
        REQUIRE(a.Except(b).Get().empty());
    }

    SECTION("Except with custom comparer")
    {
        vwr::Query a{1, 2, 3, 4};
        vwr::Query b{3, 4, 5};
        auto eq = [](const int x, const int y) { return x == y; };
        REQUIRE(a.Except(b, eq).Get() == std::vector<int>{1, 2});
    }
}

// ---------------------------------------------------------------------------
// Intersect
// ---------------------------------------------------------------------------

TEST_CASE("Query - Intersect", "[Query][Intersect]")
{
    SECTION("Intersect returns common elements")
    {
        vwr::Query a{1, 2, 3, 4};
        vwr::Query b{2, 4, 6};
        REQUIRE(a.Intersect(b).Get() == std::vector<int>{2, 4});
    }

    SECTION("Intersect with no overlap returns empty")
    {
        vwr::Query a{1, 2};
        vwr::Query b{3, 4};
        REQUIRE(a.Intersect(b).Get().empty());
    }

    SECTION("Intersect deduplicates the result")
    {
        vwr::Query a{1, 1, 2};
        vwr::Query b{1};
        REQUIRE(a.Intersect(b).Get() == std::vector<int>{1});
    }

    SECTION("Intersect on empty left returns empty")
    {
        vwr::Query a(std::vector<int>{});
        vwr::Query b{1, 2};
        REQUIRE(a.Intersect(b).Get().empty());
    }

    SECTION("Intersect with custom comparer")
    {
        vwr::Query a{1, 2, 3};
        vwr::Query b{2, 3, 4};
        auto eq = [](const int x, const int y) { return x == y; };
        REQUIRE(a.Intersect(b, eq).Get() == std::vector<int>{2, 3});
    }
}

// ---------------------------------------------------------------------------
// Join
// ---------------------------------------------------------------------------

TEST_CASE("Query - Join", "[Query][Join]")
{
    SECTION("Join correlates matching keys")
    {
        vwr::Query outer{1, 2, 3};
        vwr::Query inner{2, 3, 4};
        auto result = outer.Join(
                                inner,
                                [](const int x) { return x; },
                                [](const int y) { return y; })
                          .Get();
        REQUIRE(result == (std::vector<std::pair<int, int>>{{2, 2}, {3, 3}}));
    }

    SECTION("Join with no matching keys returns empty")
    {
        vwr::Query outer{1, 2};
        vwr::Query inner{3, 4};
        auto result = outer.Join(
                                inner,
                                [](const int x) { return x; },
                                [](const int y) { return y; })
                          .Get();
        REQUIRE(result.empty());
    }

    SECTION("Join produces a cross-product for repeated keys")
    {
        vwr::Query outer{1, 1};
        vwr::Query inner{1};
        auto result = outer.Join(
                                inner,
                                [](const int x) { return x; },
                                [](const int y) { return y; })
                          .Get();
        REQUIRE(result.size() == 2);
    }

    SECTION("Join with custom comparer")
    {
        vwr::Query outer{2, 4};
        vwr::Query inner{1, 3};
        // match when outer value is one more than inner value
        auto eq = [](const int innerKey, const int outerKey) { return outerKey - innerKey == 1; };
        auto result = outer.Join(
                                inner,
                                [](const int x) { return x; },
                                [](const int y) { return y; },
                                eq)
                          .Get();
        REQUIRE(result == (std::vector<std::pair<int, int>>{{2, 1}, {4, 3}}));
    }
}

// ---------------------------------------------------------------------------
// Reverse
// ---------------------------------------------------------------------------

TEST_CASE("Query - Reverse", "[Query][Reverse]")
{
    SECTION("Reverse inverts the sequence")
    {
        vwr::Query q{1, 2, 3, 4, 5};
        REQUIRE(q.Reverse().Get() == std::vector<int>{5, 4, 3, 2, 1});
    }

    SECTION("Reverse on single element returns same")
    {
        vwr::Query q{42};
        REQUIRE(q.Reverse().Get() == std::vector<int>{42});
    }

    SECTION("Reverse on empty returns empty")
    {
        vwr::Query q(std::vector<int>{});
        REQUIRE(q.Reverse().Get().empty());
    }

    SECTION("Double Reverse returns original sequence")
    {
        vwr::Query q{1, 2, 3};
        REQUIRE(q.Reverse().Reverse().Get() == std::vector<int>{1, 2, 3});
    }
}

// ---------------------------------------------------------------------------
// SequenceEqual
// ---------------------------------------------------------------------------

TEST_CASE("Query - SequenceEqual", "[Query][SequenceEqual]")
{
    SECTION("Equal sequences")
    {
        vwr::Query a{1, 2, 3};
        vwr::Query b{1, 2, 3};
        REQUIRE(a.SequenceEqual(b));
    }

    SECTION("Different sizes are not equal")
    {
        vwr::Query a{1, 2};
        vwr::Query b{1, 2, 3};
        REQUIRE_FALSE(a.SequenceEqual(b));
    }

    SECTION("Same size but different elements")
    {
        vwr::Query a{1, 2, 3};
        vwr::Query b{1, 2, 4};
        REQUIRE_FALSE(a.SequenceEqual(b));
    }

    SECTION("Empty sequences are equal")
    {
        vwr::Query a(std::vector<int>{});
        vwr::Query b(std::vector<int>{});
        REQUIRE(a.SequenceEqual(b));
    }

        SECTION("SequenceEqual with custom comparer")
    {
        vwr::Query<std::string> a{"Hello", "World"};
        vwr::Query<std::string> b{"hello", "world"};
        auto ci = [](const std::string& x, const std::string& y)
        {
            if(x.size() != y.size())
                return false;
            for(size_t i = 0; i < x.size(); ++i)
                if(std::tolower(static_cast<unsigned char>(x[i])) != std::tolower(static_cast<unsigned char>(y[i])))
                    return false;
            return true;
        };
        REQUIRE(a.SequenceEqual(b, ci));
    }
}

// ---------------------------------------------------------------------------
// SkipWhile
// ---------------------------------------------------------------------------

TEST_CASE("Query - SkipWhile", "[Query][SkipWhile]")
{
    SECTION("SkipWhile skips elements while predicate is true")
    {
        vwr::Query q{1, 2, 3, 4, 5};
        auto result = q.SkipWhile([](const int v) { return v < 3; }).Get();
        REQUIRE(result == std::vector<int>{3, 4, 5});
    }

    SECTION("SkipWhile skips nothing when predicate is false from the start")
    {
        vwr::Query q{5, 4, 3};
        auto result = q.SkipWhile([](const int v) { return v < 3; }).Get();
        REQUIRE(result == std::vector<int>{5, 4, 3});
    }

    SECTION("SkipWhile skips all when predicate is always true")
    {
        vwr::Query q{1, 2, 3};
        auto result = q.SkipWhile([](int) { return true; }).Get();
        REQUIRE(result.empty());
    }

    SECTION("SkipWhile does not skip elements after first false")
    {
        vwr::Query q{1, 2, 1, 3};
        auto result = q.SkipWhile([](const int v) { return v < 2; }).Get();
        REQUIRE(result == std::vector<int>{2, 1, 3});
    }
}

// ---------------------------------------------------------------------------
// TakeWhile
// ---------------------------------------------------------------------------

TEST_CASE("Query - TakeWhile", "[Query][TakeWhile]")
{
    SECTION("TakeWhile takes elements while predicate is true")
    {
        vwr::Query q{1, 2, 3, 4, 5};
        auto result = q.TakeWhile([](const int v) { return v < 4; }).Get();
        REQUIRE(result == std::vector<int>{1, 2, 3});
    }

    SECTION("TakeWhile takes nothing when predicate is false from the start")
    {
        vwr::Query q{5, 4, 3};
        auto result = q.TakeWhile([](const int v) { return v < 3; }).Get();
        REQUIRE(result.empty());
    }

    SECTION("TakeWhile takes all when predicate is always true")
    {
        vwr::Query q{1, 2, 3};
        auto result = q.TakeWhile([](int) { return true; }).Get();
        REQUIRE(result == std::vector<int>{1, 2, 3});
    }

    SECTION("TakeWhile stops at first false even if later elements pass")
    {
        vwr::Query q{1, 2, 5, 1, 2};
        auto result = q.TakeWhile([](const int v) { return v < 4; }).Get();
        REQUIRE(result == std::vector<int>{1, 2});
    }
}

// ---------------------------------------------------------------------------
// Unite
// ---------------------------------------------------------------------------

TEST_CASE("Query - Unite", "[Query][Unite]")
{
    SECTION("Unite merges two sequences keeping only unique elements")
    {
        vwr::Query a{1, 2, 3};
        vwr::Query b{2, 3, 4};
        REQUIRE(a.Unite(b).Get() == std::vector<int>{1, 2, 3, 4});
    }

    SECTION("Unite with no overlap returns all elements")
    {
        vwr::Query a{1, 2};
        vwr::Query b{3, 4};
        REQUIRE(a.Unite(b).Get() == std::vector<int>{1, 2, 3, 4});
    }

    SECTION("Unite with identical sequences returns unique elements")
    {
        vwr::Query a{1, 2, 3};
        vwr::Query b{1, 2, 3};
        REQUIRE(a.Unite(b).Get() == std::vector<int>{1, 2, 3});
    }

    SECTION("Unite with empty sequence returns non-empty unique")
    {
        vwr::Query a{1, 1, 2};
        vwr::Query b(std::vector<int>{});
        REQUIRE(a.Unite(b).Get() == std::vector<int>{1, 2});
    }
}

// ---------------------------------------------------------------------------
// Where
// ---------------------------------------------------------------------------

TEST_CASE("Query - Where", "[Query][Where]")
{
    SECTION("Where filters elements by predicate")
    {
        vwr::Query q{1, 2, 3, 4, 5, 6};
        auto result = q.Where([](const int v) { return v % 2 == 0; }).Get();
        REQUIRE(result == std::vector<int>{2, 4, 6});
    }

    SECTION("Where returns empty when no element satisfies predicate")
    {
        vwr::Query q{1, 3, 5};
        auto result = q.Where([](const int v) { return v % 2 == 0; }).Get();
        REQUIRE(result.empty());
    }

    SECTION("Where returns all when all elements satisfy predicate")
    {
        vwr::Query q{2, 4, 6};
        auto result = q.Where([](const int v) { return v % 2 == 0; }).Get();
        REQUIRE(result == std::vector<int>{2, 4, 6});
    }

    SECTION("Where on empty sequence returns empty")
    {
        vwr::Query q(std::vector<int>{});
        REQUIRE(q.Where([](int) { return true; }).Get().empty());
    }
}

// ---------------------------------------------------------------------------
// All / Any
// ---------------------------------------------------------------------------

TEST_CASE("Query - All", "[Query][All]")
{
    SECTION("All returns true when all elements satisfy predicate")
    {
        const vwr::Query q{2, 4, 6};
        REQUIRE(q.All([](int v) { return v % 2 == 0; }));
    }

    SECTION("All returns false when at least one element does not satisfy")
    {
        const vwr::Query q{2, 3, 6};
        REQUIRE_FALSE(q.All([](int v) { return v % 2 == 0; }));
    }

    SECTION("All on empty sequence returns true (vacuous truth)")
    {
        const vwr::Query q(std::vector<int>{});
        REQUIRE(q.All([](int) { return false; }));
    }
}

TEST_CASE("Query - Any", "[Query][Any]")
{
    SECTION("Any returns true when at least one element satisfies predicate")
    {
        const vwr::Query q{1, 3, 4};
        REQUIRE(q.Any([](int v) { return v % 2 == 0; }));
    }

    SECTION("Any returns false when no element satisfies predicate")
    {
        const vwr::Query q{1, 3, 5};
        REQUIRE_FALSE(q.Any([](int v) { return v % 2 == 0; }));
    }

    SECTION("Any on empty sequence returns false")
    {
        const vwr::Query q(std::vector<int>{});
        REQUIRE_FALSE(q.Any([](int) { return true; }));
    }
}

// ---------------------------------------------------------------------------
// Average
// ---------------------------------------------------------------------------

TEST_CASE("Query - Average", "[Query][Average]")
{
    SECTION("Average of integers returns double")
    {
        vwr::Query q{1, 2, 3, 4, 5};
        REQUIRE_THAT(q.Average(), Catch::Matchers::WithinRel(3.0, 1e-9));
    }

    SECTION("Average of floats")
    {
        vwr::Query q{1.0F, 2.0F, 3.0F};
        REQUIRE_THAT(q.Average(), Catch::Matchers::WithinRel(2.0F, 1e-5F));
    }

    SECTION("Average of doubles")
    {
        vwr::Query q{1.5, 2.5, 3.0};
        REQUIRE_THAT(q.Average(), Catch::Matchers::WithinRel(7.0 / 3.0, 1e-9));
    }

    SECTION("Average throws on empty sequence")
    {
        vwr::Query q(std::vector<int>{});
        REQUIRE_THROWS_AS(q.Average(), std::out_of_range);
    }

    SECTION("Average of single element")
    {
        vwr::Query q{7};
        REQUIRE_THAT(q.Average(), Catch::Matchers::WithinRel(7.0, 1e-9));
    }
}

// ---------------------------------------------------------------------------
// Contains
// ---------------------------------------------------------------------------

TEST_CASE("Query - Contains", "[Query][Contains]")
{
    SECTION("Contains returns true for present element")
    {
        vwr::Query q{1, 2, 3};
        REQUIRE(q.Contains(2));
    }

    SECTION("Contains returns false for absent element")
    {
        vwr::Query q{1, 2, 3};
        REQUIRE_FALSE(q.Contains(5));
    }

    SECTION("Contains on empty sequence returns false")
    {
        vwr::Query q(std::vector<int>{});
        REQUIRE_FALSE(q.Contains(1));
    }

    SECTION("Contains works with strings")
    {
        vwr::Query<std::string> q{"foo", "bar", "baz"};
        REQUIRE(q.Contains("bar"));
        REQUIRE_FALSE(q.Contains("qux"));
    }
}

// ---------------------------------------------------------------------------
// Max / Min
// ---------------------------------------------------------------------------

TEST_CASE("Query - Max", "[Query][Max]")
{
    SECTION("Max returns maximum element")
    {
        vwr::Query q{3, 1, 4, 1, 5, 9, 2, 6};
        REQUIRE(q.Max() == 9);
    }

    SECTION("Max on single element returns that element")
    {
        vwr::Query q{42};
        REQUIRE(q.Max() == 42);
    }

    SECTION("Max throws on empty sequence")
    {
        vwr::Query q(std::vector<int>{});
        REQUIRE_THROWS_AS(q.Max(), std::out_of_range);
    }

    SECTION("Max with negative values")
    {
        vwr::Query q{-5, -3, -1, -4};
        REQUIRE(q.Max() == -1);
    }
}

TEST_CASE("Query - Min", "[Query][Min]")
{
    SECTION("Min returns minimum element")
    {
        vwr::Query q{3, 1, 4, 1, 5, 9, 2, 6};
        REQUIRE(q.Min() == 1);
    }

    SECTION("Min on single element returns that element")
    {
        vwr::Query q{7};
        REQUIRE(q.Min() == 7);
    }

    SECTION("Min throws on empty sequence")
    {
        vwr::Query q(std::vector<int>{});
        REQUIRE_THROWS_AS(q.Min(), std::out_of_range);
    }

    SECTION("Min with negative values")
    {
        vwr::Query q{-5, -3, -1, -4};
        REQUIRE(q.Min() == -5);
    }
}

// ---------------------------------------------------------------------------
// Sum
// ---------------------------------------------------------------------------

TEST_CASE("Query - Sum", "[Query][Sum]")
{
    SECTION("Sum of integers")
    {
        vwr::Query q{1, 2, 3, 4, 5};
        REQUIRE(q.Sum() == 15);
    }

    SECTION("Sum of doubles")
    {
        vwr::Query q{1.1, 2.2, 3.3};
        REQUIRE_THAT(q.Sum(), Catch::Matchers::WithinRel(6.6, 1e-9));
    }

    SECTION("Sum throws on empty sequence")
    {
        vwr::Query q(std::vector<int>{});
        REQUIRE_THROWS_AS(q.Sum(), std::out_of_range);
    }

    SECTION("Sum of single element")
    {
        vwr::Query q{99};
        REQUIRE(q.Sum() == 99);
    }

    SECTION("Sum with negative values")
    {
        vwr::Query q{-1, -2, 3};
        REQUIRE(q.Sum() == 0);
    }
}

// ---------------------------------------------------------------------------
// Chaining
// ---------------------------------------------------------------------------

TEST_CASE("Query - Method Chaining", "[Query][Chaining]")
{
    SECTION("Where then Sum")
    {
        vwr::Query q{1, 2, 3, 4, 5, 6};
        REQUIRE(q.Where([](int v) { return v % 2 == 0; }).Sum() == 12);
    }

    SECTION("Where then Max")
    {
        vwr::Query q{1, 5, 2, 8, 3};
        REQUIRE(q.Where([](int v) { return v < 6; }).Max() == 5);
    }

    SECTION("Distinct then Reverse then Get")
    {
        vwr::Query q{3, 1, 2, 1, 3};
        auto result = q.Distinct().Reverse().Get();
        REQUIRE(result == std::vector<int>{2, 1, 3});
    }

    SECTION("Concat then Where then count via Get")
    {
        vwr::Query a{1, 2, 3};
        vwr::Query b{4, 5, 6};
        auto result = a.Concat(b).Where([](const int v) { return v > 3; }).Get();
        REQUIRE(result == std::vector<int>{4, 5, 6});
    }
}
