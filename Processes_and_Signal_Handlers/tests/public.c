#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>

#include <check.h>

#include "../signals.h"

START_TEST (UNIT_good_signals)
{
  printf ("\n======================================\n");
  printf ("UNIT good signals\n");
  printf ("This test checks for signal name lookups\n");
  char *names[] = { "INT", "KILL", "SEGV", "TSTP" };
  int signals[] = { SIGINT, SIGKILL, SIGSEGV, SIGTSTP };
  for (int i = 0; i < 4; i++)
    {
      int sig = signal_lookup (names[i]);
      ck_assert_int_eq (signals[i], sig);
    }
}
END_TEST

START_TEST (UNIT_bad_signals)
{
  printf ("\n======================================\n");
  printf ("UNIT bad signals\n");
  printf ("This test checks for other signal name lookups\n");
  char *names[] = { "HUP", "ALRM", "ILL", "KIL" };
  for (int i = 0; i < 4; i++)
    {
      int sig = signal_lookup (names[i]);
      ck_assert_int_eq (sig, -1);
    }
}
END_TEST

void public_tests (Suite *s)
{
  TCase *tc_public = tcase_create ("Public");
  tcase_add_test (tc_public, UNIT_good_signals);
  tcase_add_test (tc_public, UNIT_bad_signals);
  suite_add_tcase (s, tc_public);
}

