#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdbool.h>

// typ
typedef union
{
  int int_value;
  float float_value;
  char *string_value;
} answer_t;

typedef bool check_func(char *);
typedef answer_t convert_func(char *);

// osäker
extern char *strdup(const char *);

// läsa in
void clear_input_buffer(void);
int read_string(char *buf, int buf_siz);
void print(char *word);
void println(char *word);

// checka olika typer
bool is_number(char *str);
bool is_float(char *str);
bool not_empty(char *str);

// Konvertering
answer_t make_float(char *str);

// frågor
answer_t ask_question(char *question, check_func *check, convert_func *convert);
char ask_question_menu(char *question);
int ask_question_int(char *question);
char *ask_question_string(char *question);
double ask_question_float(char *question);
char *ask_question_shelf(char *question);

#endif