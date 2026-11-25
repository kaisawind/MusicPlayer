#include "Range.h"

#include <climits>
#include <cstdio>

static int failures = 0;

#define CHECK(expr) \
	do { if (!(expr)) { std::printf("FAIL: %s\n", #expr); ++failures; } } while (0)

int main()
{
	CHECK(ClampInt(INT_MIN, 0, 10) == 0);
	CHECK(ClampInt(-1, 0, 10) == 0);
	CHECK(ClampInt(0, 0, 10) == 0);
	CHECK(ClampInt(5, 0, 10) == 5);
	CHECK(ClampInt(10, 0, 10) == 10);
	CHECK(ClampInt(11, 0, 10) == 10);

	CHECK(NextIndex(-1, 0) == -1);
	CHECK(NextIndex(0, 0) == -1);
	CHECK(NextIndex(-1, 1) == 0);
	CHECK(NextIndex(0, 1) == 0);
	CHECK(NextIndex(0, 3) == 1);
	CHECK(NextIndex(1, 3) == 2);
	CHECK(NextIndex(2, 3) == 2);
	CHECK(NextIndex(99, 3) == 2);

	CHECK(PrevIndex(-1, 0) == -1);
	CHECK(PrevIndex(0, 0) == -1);
	CHECK(PrevIndex(-1, 2) == 0);
	CHECK(PrevIndex(0, 2) == 0);
	CHECK(PrevIndex(1, 2) == 0);
	CHECK(PrevIndex(2, 2) == 0);
	CHECK(PrevIndex(2, 3) == 1);
	CHECK(PrevIndex(99, 2) == 0);

	if (0 == failures)
	{
		std::printf("all tests passed\n");
		return 0;
	}

	std::printf("%d tests failed\n", failures);
	return 1;
}
