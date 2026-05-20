// File: TypingTest.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LINE_LEN 80
#define WPM_WORD_LEN 5


int errmsg(const char * msg)
{
  fprintf(stderr, "ERROR: %s\n", msg);
  return 1;
}


void clear_stdin(const char * usr_buf)
{
  for (int i = 0; i < LINE_LEN && usr_buf[i] != '\0'; i++)
  {
    if (usr_buf[i] == '\n') return;
  }
  while (getchar() != '\n');
}


void generate_text(char * cpu_buf)
{
  unsigned int space_odds = 0;
  for (int i = 0; i < LINE_LEN - 1; i++)
  {
    if (arc4random_uniform(100) < space_odds)
    {
      cpu_buf[i] = ' ';
      space_odds = 0;
    }
    else
    {
      cpu_buf[i] = 'a' + arc4random_uniform(26);
      space_odds += 10;
    }
  }
  cpu_buf[LINE_LEN - 1] = 'a' + arc4random_uniform(26);
  cpu_buf[LINE_LEN] = '\0';
}


int count_mistakes(const char * s1, const char * s2)
{
  int mistakes = 0;
  for (int i = 0; i < LINE_LEN; i++)
  {
    if (s1[i] == '\0' || s2[i] == '\0')
    {
      mistakes += LINE_LEN - i;
      break;
    }
    mistakes += (s1[i] != s2[i]);
  }
  return mistakes;
}


int main()
{
  char cpu_buf[LINE_LEN + 1];
  char usr_buf[LINE_LEN + 1];

  puts("How many lines do you want to type?\n");
  fgets(usr_buf, LINE_LEN, stdin);
  int num_lines = atoi(usr_buf);
  if (num_lines < 1) return errmsg("Invalid line count");
  clear_stdin(usr_buf);

  int total_mistakes = 0;
  struct timespec start_time;
  clock_gettime(CLOCK_REALTIME, &start_time);

  for (int loop = 0; loop < num_lines; loop++)
  {
    generate_text(cpu_buf);
    printf("\n%s\n\n", cpu_buf);

    fgets(usr_buf, LINE_LEN + 1, stdin);
    int mistakes = count_mistakes(cpu_buf, usr_buf);
    total_mistakes += mistakes;
    printf("\n%d mistakes\n", mistakes);
    clear_stdin(usr_buf);
  }

  struct timespec end_time;
  clock_gettime(CLOCK_REALTIME, &end_time);
  time_t total_secs = end_time.tv_sec - start_time.tv_sec;
  time_t total_msecs = (end_time.tv_nsec - start_time.tv_nsec) / 1000000;
  if (total_msecs < 0)
  {
    total_secs--;
    total_msecs += 1000;
  }

  double mistake_ratio = total_mistakes / (double) (LINE_LEN * num_lines);
  double total_accuracy = 100.0 - mistake_ratio * 100.0;

  double words_typed = (double) (LINE_LEN * num_lines) / WPM_WORD_LEN;
  double wpm = words_typed * 60.0 / (total_secs + total_msecs / 1000.0);

  puts("\nTest complete!");
  printf("Total mistakes: %d\n", total_mistakes);
  printf("Total accuracy: %.2lf%%\n", total_accuracy);
  printf("Time spent: %ld.%ld seconds\n", total_secs, total_msecs);
  printf("Words per minute: %.1lf\n", wpm);
  return 0;
}
