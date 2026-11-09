/*
 * C testbench for mac().
 *
 * Reads the test cases from in.dat, runs mac() on each one and compares the
 * results with out.gold.dat (one expected value per line). All results are
 * written to LOG_FILE; nothing is printed to the console. The Makefile sets
 * LOG_FILE per implementation (dsp_test.log, pl_test.log).
 *
 * Returns 0 if every test case passes and 1 otherwise, so Vitis HLS C
 * simulation and C/RTL co-simulation report the result correctly.
 */
#include <stdio.h>
#include <stdint.h>
#include "dsp_vs_pl.h"

#define IN_FILE   "in.dat"
#define GOLD_FILE "out.gold.dat"
#ifndef LOG_FILE
#define LOG_FILE  "dsp_vs_pl_test.log"
#endif

void mac(int in1[SIZE], int in2[SIZE], int *out);

/*
 * Read the next integer from fp, skipping whitespace and '#' comments
 * (a '#' comments out the rest of its line).
 * Returns 1 if an integer was read, 0 at end of file or on bad input.
 */
static int read_int(FILE *fp, int *val)
{
  int c;

  for (;;)
  {
    c = fgetc(fp);
    if (c == EOF)
      return 0;
    if (c == '#')
    {
      while ((c = fgetc(fp)) != '\n' && c != EOF)
        ;
      continue;
    }
    if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
      continue;

    ungetc(c, fp);
    return fscanf(fp, "%d", val) == 1;
  }
}

static int read_array(FILE *fp, int arr[SIZE])
{
  int i;

  for (i = 0; i < SIZE; i++)
    if (!read_int(fp, &arr[i]))
      return 0;
  return 1;
}

int main(void)
{
  static int in1[SIZE];
  static int in2[SIZE];
  int out, gold;
  int num_tests, t;
  int errors = 0;
  FILE *fin, *fgold, *flog;

  flog = fopen(LOG_FILE, "w");
  if (flog == NULL)
    return 1;  /* nowhere to report to; the non-zero exit code flags it */

  fin = fopen(IN_FILE, "r");
  if (fin == NULL)
  {
    fprintf(flog, "ERROR: cannot open %s\n", IN_FILE);
    fclose(flog);
    return 1;
  }

  fgold = fopen(GOLD_FILE, "r");
  if (fgold == NULL)
  {
    fprintf(flog, "ERROR: cannot open %s\n", GOLD_FILE);
    fclose(fin);
    fclose(flog);
    return 1;
  }

  if (!read_int(fin, &num_tests) || num_tests <= 0)
  {
    fprintf(flog, "ERROR: cannot read the number of test cases from %s\n", IN_FILE);
    fclose(fin);
    fclose(fgold);
    fclose(flog);
    return 1;
  }

  for (t = 1; t <= num_tests; t++)
  {
    if (!read_array(fin, in1) || !read_array(fin, in2))
    {
      fprintf(flog, "ERROR: %s ended early in test case %d (expected %d values per array)\n",
             IN_FILE, t, SIZE);
      errors++;
      break;
    }

    if (fscanf(fgold, "%d", &gold) != 1)
    {
      fprintf(flog, "ERROR: %s has no expected value for test case %d\n", GOLD_FILE, t);
      errors++;
      break;
    }

    mac(in1, in2, &out);

    if (out == gold)
    {
      fprintf(flog, "Test case %2d: PASS (out = %d)\n", t, out);
    }
    else
    {
      fprintf(flog, "Test case %2d: FAIL (out = %d, expected = %d)\n", t, out, gold);
      errors++;
    }
  }

  fclose(fin);
  fclose(fgold);

  fprintf(flog, "------------------------------------------------\n");
  if (errors == 0)
    fprintf(flog, "TEST PASSED: all %d test cases match %s\n", num_tests, GOLD_FILE);
  else
    fprintf(flog, "TEST FAILED: %d error(s)\n", errors);
  fprintf(flog, "------------------------------------------------\n");

  fclose(flog);
  return errors ? 1 : 0;
}

