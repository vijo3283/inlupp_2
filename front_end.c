#include"front_end.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"



void print_menu(void)
{
    char *A = "[A]dd merchandise";
    char *L = "[L]ist merchandise";
    char *H = "S[H]ow one merchandise";
    char *D = "[D]elete merchandise";
    char *E = "[E]dit merchandise";
    char *S = "[S]how stock merchandise";
    char *P = "Re[p]lenish"; 
    char *C = "[C]reate Cart";
    char *R = "[R]emove Cart";
    char *add_cart = "[+]Add to Cart";
    char *remove_cart = "[-]Remove from Cart";
    char *calculate = "[=]Calculate cost";
    char *O = "Check[O]ut"; 
    char *Q = "[Q]uit";

    printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n", 
        A, L, H, D, E, S, P, C, R, add_cart, remove_cart, calculate, O, Q);
}

bool confirmation_ui_yes(void)
{
    char *answer = ask_question_string("Is this correct? (Yes) \n");

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
    char *answer = ask_question_string("Do you want to continue? (No)\n");

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

    char *name = ask_question_string("What is the name of the merch \n");
    char *description = ask_question_string("What is the description \n");

    int price;
    do
    {
        price = ask_question_int("What is the price \n");
    }
    while(!is_positive_number(price));

    merch_t *merch = ioopm_create_merchandise(name, description, price);
    ioopm_add_merchandise(db->merch_ht, merch);
}

void list_merchandise_ui(data_base_t *db){
    size_t index = 0;

    bool continue_list = list_merchandise(db, index);

    if(continue_list)
    {
        while (!confirmation_ui_no())
        {
        continue_list = list_merchandise(db, index + 20);
        }
        
    }

    return;

    
}

void list_one_merchandise_ui(data_base_t *db)
{
    char *merch = ask_question_string("Which merch do you want to list: \n");
    list_one_merch(db,merch);
}


void remove_merchandise_ui(data_base_t *db)
{
    char *merch = ask_question_string("Which merch do you want to remove?\n");
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
    char *merch = ask_question_string("Which merch do you want to edit?\n");
    //kanske göra en lookup?
    char *name = ask_question_string("What is the name of the merch\n");
    char *description = ask_question_string("What is the description\n");

    int price;
    do
    {
        price = ask_question_int("What is the new price\n");
    }
    while(!is_positive_number(price));
    

    
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
    char *name = ask_question_string("Name of merch:\n");
    size_t stock = show_stock(db, name);
    printf("%s has %zu merchandies in stock\n", name, stock);
    
}

void replenish_ui(data_base_t *db)
{
    char *name = ask_question_string("Name of merch:\n");
    int amount;
    do
    {
        amount = ask_question_int("How many?\n");
    }
    while(!is_positive_number(amount));

    
    char *shelf = ask_question_shelf("Which shelf?\n");
   
    replenish(db, name, amount, shelf);
}

void create_cart_ui(data_base_t *db)
{
    int user_id = create_cart(db);
    printf("Your cart ID is: %d\n", user_id);

}

void remove_cart_ui(data_base_t *db){
    int user_id = ask_question_int("What is your cart ID: \n");
    remove_cart(db, user_id);
}

void add_to_cart_ui(data_base_t *db){
    int user_id = ask_question_int("What is your cart ID: \n");
    char *add_merch = ask_question_string("Which merch do you want to add: \n");
    int amount;
    do
    {
        amount = ask_question_int("How many: \n");
    }
    while(!is_positive_number(amount));

    add_to_cart(db, user_id, add_merch, amount);
}

void remove_from_cart_ui(){
    //TODO:stub
}

void calculate_cost_ui(data_base_t *db){
    int user_id = ask_question_int("What is your cart ID: \n");

   int cost = calculate_cost(db, user_id);
   printf("Your total is: %d \n", cost);
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
        printf("\n");
        print_menu();
        printf("\n");
        char choice = ask_question_menu("Pick your choice: \n");
        if (choice == 'A'){
            add_merchandise_ui(db);
        }
        else if (choice == 'L'){
            list_merchandise_ui(db); //LÖS
        }    
        else if (choice == 'H'){
            list_one_merchandise_ui(db); //LÖS    
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
            create_cart_ui(db);
        }
        else if (choice == 'R') {
            remove_cart_ui(db);
        }
        else if (choice == '+') {
            add_to_cart_ui(db);
        }
        else if (choice == '-') {
            remove_from_cart_ui(db);
        }
        else if (choice == '=') {
            calculate_cost_ui(db);
        }
        else if (choice == 'O') {
            checkout_ui(db);
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