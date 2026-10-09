#include <stdio.h>
#include "utils.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>


//en extern funktion som finns i ett bibliotek.
extern char *strdup(const char *);


void clear_input_buffer(void)
{
    int c;
      
    do
    {
        c = getchar();
    }
    while (c != '\n' && c != EOF);

}

int read_string(char *buf, int buf_siz)
{
    int count = 0;
    int c;

    c = getchar();
    
    while (c != '\n' && c != EOF){
        if(count < buf_siz-1)
        {
            buf[count] = c;
            count += 1;
        }
        else 
        {
            clear_input_buffer();
            break;

        }

        c = getchar();
    }

    buf[count] = '\0';
    return count;

}

bool is_number(char *str)
{
    int len = strlen(str);

    if(len == 0) 
    {
        return false;

    }

    int beginning = 0;

    if (str[0] == '-') {
        if(len == 1){
            return false;
        }
        beginning = 1;
    }

    for(int i = beginning; i < len; i++)
    {
        if(!isdigit(str[i])){
            return false;
        }

    }

    return true;

    
}

void print(char *word){

    bool terminator = true;

    for(int i = 0; terminator; i++){
        if(word[i] == '\0'){
            terminator = false;
        }
        else 
        {
            putchar(word[i]);
        }
    }
}

void println(char *word){
    print(word);
    putchar('\n');

}

bool is_float(char *str)
{
  int len = strlen(str);

  if (len == 0)
  {
    return false;
  }

  //kopierad från AI eftersom jag ej har gjort extra uppgiften
  int start = 0;
  // Tillåt ett inledande plustecken eller minustecken
  if (str[0] == '-' || str[0] == '+')
  {
    if (len == 1) // Enbart tecknet är inte ett giltigt tal
    {
      return false;
    }
    start = 1;
  }

  bool seen_dot = false;
  bool seen_digit = false;

  for (int i = start; i < len; ++i)
  {
    if (str[i] == '.')
    {
      // Fler än en punkt är ogiltigt
      if (seen_dot)
      {
        return false;
      }
      seen_dot = true;
    }
    else if (isdigit(str[i]))
    {
      seen_digit = true;
    }
    else
    {
      // Alla andra tecken är ogiltiga
      return false;
    }
  }

  // Strängen måste innehålla minst en siffra (t.ex. är "." ogiltigt)
  return seen_digit;
}

//kopierad från labb
answer_t make_float(char *str)
{
  answer_t a;                // skapa ett oinitierat answer_t-värde
  a.float_value = atof(str); // gör det till en float, via atof
  return a;                  // returnera värdet
}

/// Hjälpfunktion till ask_question_string, kopierad från labb
bool not_empty(char *str)
{
  return strlen(str) > 0;
}


answer_t ask_question(char *question, check_func *check, convert_func *convert)
{
    int buf_siz = 255;
    char buf[buf_siz];

    do
    {
        print(question);
        read_string(buf, buf_siz);

    }

    while(!check(buf));
   
    return convert(buf);

}

//check if the str is correct, använd i asked question menu
bool right_letter(char *str)
{
    //så att den är exakt en lång
    if (strlen(str) != 1)
    {
        return false;
    }

    //sätter den till stor
    char val = toupper(str[0]);

    //Returns a pointer to the first occurrence of the character if it is found.
    if (strchr("ALDESPCR", val) != NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

answer_t char_to_answer(char *str)
{
    //ändrar parametern str till typen answer_t som answer question behöver
    //gör också den till en stor bokstav och lägger in den som ascii nummer
    return (answer_t){ .int_value = toupper(str[0]) };

}

char *ask_question_string(char *question)
{
  answer_t answer = ask_question(question, not_empty, (convert_func *) strdup);
  
  return answer.string_value;

}


//kopierad från labb
int ask_question_int(char *question)
{
  answer_t answer = ask_question(question, is_number, (convert_func *) atoi);
  return answer.int_value; // svaret som ett heltal
}


double ask_question_float(char *question)
{
  //kortare varianten
  return ask_question(question, is_float, make_float).float_value;
}

bool is_shelf(char *shelf)
{
  int len = strlen(shelf);

  if(len != 3)
  {
    return false;

  }

  if(!isalpha(*shelf)){
    return false;
  }

  shelf ++;

  while(*shelf != '\0')
  {
    if(!isdigit(*shelf))
    {
      return false;

    }

    shelf++;

  }

  return true;

}

char *ask_question_shelf(char *question)
{
  answer_t answer = ask_question(question, (check_func *) is_shelf, (convert_func *) strdup);
  return answer.string_value;


}




char ask_question_menu(char *question)
{
    //tar hjälp av ask_question
    answer_t answer = ask_question(question, (check_func *) right_letter, (convert_func *) char_to_answer);

    //gör den till char eftersom den är skriven som ascii
    return (char)answer.int_value;
}

