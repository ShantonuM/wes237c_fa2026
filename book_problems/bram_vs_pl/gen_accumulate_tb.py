#!/usr/bin/env python3
"""
Generate testbench input (in.dat) and golden output (out.gold.dat) files
for the HLS accumulate function:

    void accumulate(int in[SIZE], int *out);
    *out = sum(in[i]) for i in [0, SIZE)

in.dat format:
    <N> # Number of test cases
    # Test case 1
    <SIZE ints> # in
    # Test case 2
    ...

out.gold.dat format: one result per line, nothing else.

Random values are bounded so that no partial sum can exceed the signed
32-bit int range: SIZE * max|value| <= INT32_MAX.
"""

import argparse
import random

INT32_MAX = 2**31 - 1


def main():
    p = argparse.ArgumentParser(description=__doc__,
                                formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("-n", "--num-tests", type=int, default=10, help="number of test cases (default 10)")
    p.add_argument("-s", "--size", type=int, default=1024, help="SIZE, elements per array (default 1024)")
    p.add_argument("--max-abs", type=int, default=None,
                   help="values are drawn from [-MAX_ABS, MAX_ABS]; default and upper limit is the "
                        "largest value that cannot overflow a 32-bit int for this SIZE")
    p.add_argument("--seed", type=int, default=1, help="RNG seed, for reproducible files (default 1)")
    p.add_argument("--in-file", default="in.dat")
    p.add_argument("--gold-file", default="out.gold.dat")
    args = p.parse_args()

    # Worst case: every value is max_abs with the same sign, SIZE times.
    safe_max = INT32_MAX // args.size
    max_abs = safe_max if args.max_abs is None else args.max_abs
    if not (0 <= max_abs <= safe_max):
        p.error(f"--max-abs must be between 0 and {safe_max} for SIZE={args.size} "
                f"to guarantee no 32-bit overflow")

    rng = random.Random(args.seed)

    with open(args.in_file, "w") as fin, open(args.gold_file, "w") as fgold:
        fin.write(f"{args.num_tests} # Number of test cases\n")

        for t in range(1, args.num_tests + 1):
            data = [rng.randint(-max_abs, max_abs) for _ in range(args.size)]
            out = sum(data)
            assert -INT32_MAX - 1 <= out <= INT32_MAX

            fin.write(f"# Test case {t}\n")
            fin.write(" ".join(map(str, data)) + " # in\n")
            fgold.write(f"{out}\n")

    print(f"Wrote {args.num_tests} test cases (SIZE={args.size}, values in "
          f"[-{max_abs}, {max_abs}]) to {args.in_file} and {args.gold_file}")


if __name__ == "__main__":
    main()
