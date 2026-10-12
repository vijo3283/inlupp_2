#include"front_end.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"



void print_menu(void)
{
    char *A = "[A]dd merchandise";
    char *L = "[L]ist merchandise";
    char *D = "[D]elete merchandise";
    char *E = "[E]dit merchandise";
    char *S = "[S]how merchandise";
    char *P = "Re[p]lenish"; 
    char *C = "[C]reate Cart";
    char *R = "[R]emove Cart";
    char *add_cart = "[+]Add to Cart";
    char *remove_cart = "[-]Remove from Cart";
    char *calculate = "[=]Calculate cost";
    char *O = "Check[O]ut"; 
    char *Q = "[Q]uit";

    printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n", 
        A, L, D, E, S, P, C, R, add_cart, remove_cart, calculate, O, Q);
}

bool confirmation_ui_yes(void)
{
    char *answer = ask_question_string("Is this correct? (Yes)");

    if(answer[0] == 'Y' || answer[0] == 'y')
    {
        return true;
    }
    else
    {
        return false;
    }
    
}

bool confirmation_ui_no(void)
{
    char *answer = ask_question_string("Do you want to continue? (No)");

    if(answer[0] == 'N' || answer[0] == 'n')
    {
        return true;
    }
    else
    {
        return false;
    }
}

void add_merchandise_ui(data_base_t *db)
{

    char *name = ask_question_string("What is the name of the merch");
    char *description = ask_question_string("What is the description");
    int price = ask_question_int("What is the description");
    merch_t *merch = ioopm_create_merchandise(name, description, price);
    ioopm_add_merchandise(db->merch_ht, merch);
}

void list_merchandise_ui(data_base_t *db){
    size_t index = 0;
    list_merchandise(db, index);
    while (!confirmation_ui_no())
    {
        list_merchandise(db, index + 20);
    }
}

void remove_merchandise_ui(data_base_t *db)
{
    char *merch = ask_question_string("Which merch do you want to remove?");
    if (confirmation_ui_yes())
    {
        remove_merchandise(db, merch);
    }
    else 
    {
        remove_merchandise_ui(db);
    }
}

void edit_merchandise_ui(data_base_t *db)
{
    char *merch = ask_question_string("Which merch do you want to edit?");
    char *name = ask_question_string("What is the name of the merch");
    char *description = ask_question_string("What is the description");
    size_t price = ask_question_int("What is the new price");
    if (confirmation_ui_yes())
    {
        edit_merchandise(db, merch, name, description, price);
    }
    else 
    {
        edit_merchandise_ui(db);
    }
}

void show_stock_ui(data_base_t *db)
{
    char *name = ask_question_string("Name of merch:");
    show_stock(db, name);
}

void replenish_ui(data_base_t *db)
{
    char *name = ask_question_string("Name of merch:");
    size_t amount = ask_question_int("How many?");
    char *shelf = ask_question_shelf("Which shelf?");
   
    replenish(db, name, amount, shelf);
}

void create_cart_ui()
{
    //TODO:stub
}

void remove_cart_ui(){
    //TODO:stub
}

void add_to_cart_ui(){
    //TODO:stub
}

void remove_from_cart_ui(){
    //TODO:stub
}

void calculate_cost_ui(){
    //TODO:stub
}

void checkout_ui(){
    //TODO:stub
}

bool quit(void)
{
    return confirmation_ui_yes();
}


static int event_loop() {
    
    data_base_t *db = ioopm_create_data_base();
    
    while(true)
    {
        print_menu();
        char choice = ask_question_menu("Pick your choice: ");
        if (choice == 'A'){
            add_merchandise_ui(db);
        }
        else if (choice == 'L'){
            list_merchandise_ui(db); //LÖS
        }
        else if (choice == 'D'){
            remove_merchandise_ui(db);
        }
        else if (choice == 'E'){
            edit_merchandise_ui(db);
        }
        else if (choice == 'S') {
            show_stock_ui(db);
        }
        else if (choice == 'P') {
            replenish_ui(db);
        }
        else if (choice == 'C') {
            create_cart_ui();
        }
        else if (choice == 'R') {
            remove_cart_ui();
        }
        else if (choice == '+') {
            add_to_cart_ui();
        }
        else if (choice == '-') {
            remove_from_cart_ui();
        }
        else if (choice == '=') {
            calculate_cost_ui();
        }
        else if (choice == 'O') {
            checkout_ui();
        }
        else if (choice == 'Q') {
            if(quit())
            {
                //returnera 0 eller 1 eller break?
                break;

            }
            
        }
    }
    
    return 0;
    
}



int main(int argc, char* argv[])
{
    (void) argc;
    (void) argv;
    event_loop();

}