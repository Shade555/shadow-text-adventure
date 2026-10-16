#define _POSIX_C_SOURCE 199309L

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct {
    bool lantern;
    bool mushroom;
    bool rope;
} Inventory;

static void type_text(const char *text, unsigned int delay_us) {
    for (size_t i = 0; text[i] != '\0'; ++i) {
        putchar(text[i]);
        fflush(stdout);
        struct timespec delay = {
            .tv_sec = delay_us / 1000000U,
            .tv_nsec = (long)(delay_us % 1000000U) * 1000L
        };
        nanosleep(&delay, NULL);
    }
}

static char get_choice(const char *valid_choices) {
    while (1) {
        int ch = getchar();
        int discard;
        if (ch == EOF) {
            return '\0';
        }

        while ((discard = getchar()) != '\n' && discard != EOF) {
        }

        ch = toupper((unsigned char)ch);

        if (strchr(valid_choices, ch) != NULL) {
            return (char)ch;
        }

        type_text("\nInvalid choice. Please enter one of the listed options: ", 8000);
    }
}

static void show_inventory(const Inventory *inv) {
    type_text("\nInventory: ", 6000);
    if (!inv->lantern && !inv->mushroom && !inv->rope) {
        type_text("(empty)\n", 6000);
        return;
    }

    bool first = true;
    if (inv->lantern) {
        type_text("lantern", 6000);
        first = false;
    }
    if (inv->mushroom) {
        type_text(first ? "mooncap mushroom" : ", mooncap mushroom", 6000);
        first = false;
    }
    if (inv->rope) {
        type_text(first ? "rope" : ", rope", 6000);
    }
    type_text("\n", 6000);
}

static void win_ending(const Inventory *inv) {
    type_text("\nYou discover an ancient stone gate covered in symbols.\n", 7000);
    if (inv->mushroom && inv->lantern) {
        type_text("The mushroom glows against the lantern light, revealing a hidden keyhole.\n", 7000);
        type_text("The gate opens, and you step into the lost sanctuary of dawn. You win!\n", 7000);
    } else {
        type_text("Without enough clues, the gate stays sealed and the forest stays silent.\n", 7000);
        type_text("You survive the night, but the mystery remains unsolved.\n", 7000);
    }
}

static void lose_ending(void) {
    type_text("\nThe current grabs your legs and pulls you under.\n", 7000);
    type_text("The forest keeps your story, unfinished. You lose.\n", 7000);
}

static void open_ending(const Inventory *inv) {
    (void)inv;
    type_text("\nAt dawn, you find a hidden trail leading beyond the known map.\n", 7000);
    type_text("A distant city glitters on the horizon. Your adventure continues...\n", 7000);
}

static void mushroom_path(Inventory *inv) {
    type_text("\nYou follow Path A. Glowing mushrooms pulse between twisted roots.\n", 7000);
    type_text("You find an old lantern beside a stump and a rare mooncap mushroom.\n", 7000);
    inv->lantern = true;
    inv->mushroom = true;

    show_inventory(inv);

    type_text("\nA narrow passage opens ahead:\n", 7000);
    type_text("A) Enter the stone archway\n", 7000);
    type_text("B) Return to the crossroads\n> ", 7000);

    char choice = get_choice("AB");
    if (choice == 'A') {
        win_ending(inv);
    } else {
        open_ending(inv);
    }
}

static void river_path(Inventory *inv) {
    type_text("\nYou follow Path B. A rushing river cuts through black stone.\n", 7000);
    type_text("A coil of sturdy rope hangs from a branch.\n", 7000);
    inv->rope = true;

    show_inventory(inv);

    type_text("\nThe water roars louder as night falls:\n", 7000);
    type_text("A) Use the rope to cross near the rapids\n", 7000);
    type_text("B) Follow the bank downstream\n> ", 7000);

    char choice = get_choice("AB");
    if (choice == 'A') {
        lose_ending();
    } else {
        open_ending(inv);
    }
}

static void start_game(void) {
    Inventory inventory = {0};

    type_text("Welcome to Shadow Forest.\n", 9000);
    type_text("You stand at a mysterious crossroads beneath a moonless sky.\n", 9000);
    type_text("\nChoose your path:\n", 9000);
    type_text("A) Follow the glowing mushrooms\n", 9000);
    type_text("B) Head toward the rushing river\n> ", 9000);

    char first_choice = get_choice("AB");
    if (first_choice == 'A') {
        mushroom_path(&inventory);
    } else {
        river_path(&inventory);
    }
}

int main(void) {
    start_game();
    return 0;
}
