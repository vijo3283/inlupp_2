#include"front_end.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"



void print_menu(){
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

bool confirmation_ui(void){
    char *answer = ask_question_string("Is this correct?");
    char choice = toupper(answer[0]);

    if(strchr("Y", choice) != NULL)
    {
        return true;
    }
    else
    {
        return false;
    }

}



void add_merchandise_ui(data_base_t *db){

    char *name = ask_question_string("What is the name of the merch");
    char *description = ask_question_string("What is the description");
    int price = ask_question_int("What is the description");
    merch_t *merch = ioopm_create_merchandise(name, description, price);
    ioopm_add_merchandise(db->merch_ht, merch);
}

void remove_merchandise_ui(data_base_t *db){
    char *merch = ask_question_string("Which merch do you want to remove?");
    if (confirmation_ui())
    {
        remove_merchandise(db, merch);
    }
    else 
    {
        remove_merchandise_ui(db);
    }
}
void edit_merchandise_ui(data_base_t *db){
    char *merch = ask_question_string("Which merch do you want to edit?");
    char *name = ask_question_string("What is the name of the merch");
    char *description = ask_question_string("What is the description");
    size_t price = ask_question_int("What is the new price");
    if (confirmation_ui())
    {
        edit_merchandise(db, merch, name, description, price);
    }
    else 
    {
        edit_merchandise_ui(db);
    }
}
void show_stock_ui(data_base_t *db){
    char *name = ask_question_string("Name of merch:");
    show_stock(db, name);
}
void replenish_ui(data_base_t *db){
    char *name = ask_question_string("Name of merch:");
    size_t amount = ask_question_int("How many?");
    char *shelf = ask_question_shelf("Which shelf?");
   
    replenish(db->merch_ht, name, amount, shelf);
}
void create_cart_ui(){
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



int event_loop(hash_table_t *ht) {
    
    data_base_t *db = ioopm_create_data_base();
    
    while(true){
        print_menu();
        char choice = ask_question_menu("Pick your choice: ");
        if (choice == 'A'){
            add_merchandise_ui(&db);
        }
        else if (choice == 'L'){
            list_merchandise(&db); //LÖS
        }
        else if (choice == 'D'){
            remove_merchandise_ui(&db);
        }
        else if (choice == 'E'){
            edit_merchandise_ui(ht);
        }
        else if (choice == 'S') {
            show_stock_ui(ht);
        }
        else if (choice == 'P') {
            replenish_ui(ht);
            return 0;
        }
        else if (choice == 'C') {
            create_cart_ui();
            return 0;
        }
        else if (choice == 'R') {
            remove_cart_ui();
            return 0;
        }
        else if (choice == '+') {
            add_to_cart_ui();
            return 0;
        }
        else if (choice == '-') {
            remove_from_cart_ui();
            return 0;
        }
        else if (choice == '=') {
            calculate_cost_ui();
            return 0;
        }
        else if (choice == 'O') {
            checkout_ui();
            return 0;
        }
        else if (choice == 'Q') {
            quit();
            return 0;
        }
    }
    return 0;
    
}