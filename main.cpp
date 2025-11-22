#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;
// Globals for actions
int HUNT_ENERGY_REQ = 10;
int GATHER_ENERGY_REQ = 15;
int EXPLORE_ENERGY_REQ = 20;

//Items
struct Item {
    string name;
    int durability;
    int breakChance;
    //Boosts
    int huntBoost;        // increases success or food gain
    int exploreSafety;    // reduces chance of danger
    bool protectsFromAnimals; // avoids animal attack events

    bool isBroken() const { // To check if item is broken
        return durability <= 0;
    }
};
//Player stats
struct Player {
    int health;
    int hunger;
    int energy;
    int daysSurvived;

    Item inventory[5];
    int itemCount;    // how many items are actually in the array
};

int randomInt(int min, int max);

//Menu
void showMainMenu();
void showInstructions();

//Game
void startGame();
void dailyStatus(const Player &p);
int chooseAction(const Player &p);
void processAction(int choice, Player &p);
void randomEvent(Player &p);
void useItem(Item &item);
// Actions
void hunt(Player &p);
void gather(Player &p);
void rest(Player &p);
void explore(Player &p);


int main()
{
    srand(time(0));

    int menuChoice;

    do {
        showMainMenu();
        cin >> menuChoice;

        switch(menuChoice) {
            case 1:
                startGame();
                break;
            case 2:
                showInstructions();
                break;
            case 3:
                cout << "Exiting game... Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Try again." << endl;
        }

    } while(menuChoice != 3); // runs while not choosing to exit


    return 0;
}
// Menu
void showMainMenu(){
    cout << "\n===== Survival Simulator =====\n";
    cout << "1. Start New Game" << endl;
    cout << "2. Instructions" << endl;
    cout << "3. Exit" << endl;
    cout << "Choose an option: ";
}
void showInstructions()
{
    cout << "You were flying in a plane to see your long lost brother,then the plane suddenly crashed, you got stranded on an unknown island." << endl;
    cout << "Your goal is to survive as many days as possible by managing your hunger,energy and health" << endl;
    cout << "Choose your actions wisely" << endl;
}
//Game start
void startGame(){
    Player p = {randomInt(50,100), randomInt(50,100), randomInt(50,100), 0};
    p.itemCount = 0;
    int action;

    cout << "\nStarting new game..." << endl;
    // Give player a spear in slot 0
    p.inventory[p.itemCount++] = {"Spear", randomInt(1,100), 30, 20, 0, true};

    while(p.health > 0) {
        p.daysSurvived++;

        dailyStatus(p);
        action = chooseAction(p);
        processAction(action, p);

        if (action != 3) { // while resting random events don't occur
            randomEvent(p);
        }

        // so no value goes over or under the min, max
        if (p.hunger < 0) p.hunger = 0;
        if (p.energy < 0) p.energy = 0;
        if (p.health < 0) p.health = 0;
        if (p.hunger > 100) p.hunger = 100;
        if (p.energy > 100) p.energy = 100;
        if (p.health > 100) p.health = 100;

        cout << "\nEnd of Day " << p.daysSurvived << ". Press Enter to continue...";
        cin.ignore();
        cin.get();
    }

    cout << "\nGAME OVER! You survived " << p.daysSurvived << " days." << endl;
}
void dailyStatus(const Player &p) {
    cout << "\n===== Day " << p.daysSurvived << " =====" << endl;
    cout << "Health: " << p.health << endl;
    cout << "Hunger: " << p.hunger << endl;
    cout << "Energy: " << p.energy << endl;
    cout << "Items: " << endl;
    if (p.itemCount == 0)
        cout << " - None" << endl;
    else {
        for (int i = 0; i < p.itemCount; i++) {
            cout << " - " << p.inventory[i].name << " (Durability: " << p.inventory[i].durability << "%)"<< (p.inventory[i].isBroken() ? " [BROKEN]" : "") << endl;
        }
    }
}
int chooseAction(const Player &p) {
    cout << "What will you do?" << endl;

    cout << "1. Hunt";
    if (p.energy < HUNT_ENERGY_REQ)
        cout << " (Not enough energy)" << endl;

    cout << "2. Gather Resources";
    if (p.energy < GATHER_ENERGY_REQ)
        cout << " (Not enough energy)" << endl;

    cout << "3. Rest" << endl;

    cout << "4. Explore";
    if (p.energy < EXPLORE_ENERGY_REQ)
        cout << " (Not enough energy)" << endl;

    cout << "Choose: ";

    int input;
    cin >> input;
    return input;
}
void processAction(int choice, Player &p) {
    switch(choice) {

        case 1: // Hunt
            if (p.energy < HUNT_ENERGY_REQ) {
                cout << "You are too tired to hunt! (Need at least " << HUNT_ENERGY_REQ << " energy)" << endl;
                return;
            }
            hunt(p);
            break;

        case 2: // Gather
            if (p.energy < GATHER_ENERGY_REQ) {
                cout << "You don't have enough energy to gather resources! (Need at least " << GATHER_ENERGY_REQ << " energy)" << endl;
                return;
            }
            gather(p);
            break;

        case 3: // Rest
            rest(p);
            break;

        case 4: // Explore
            if (p.energy < EXPLORE_ENERGY_REQ) {
                cout << "You are too exhausted to explore! (Need at least " << EXPLORE_ENERGY_REQ << " energy)" << endl;;
                return;
            }
            explore(p);
            break;

        default:
            cout << "Invalid choice. You lose the day." << endl;;
    }
}
void useItem(Item &item) {
    int roll = randomInt(1, 100);
    if (roll <= item.breakChance) {
        item.durability -= randomInt(1, 100);

        if (item.durability < 0)
            item.durability = 0;

        cout << item.name << " lost durability! (" << item.durability << "% remaining)" << endl;

        if (item.isBroken()) {
            cout << "Your " << item.name << " broke!" << endl;
        }
    }
}
int chooseItem(Player &p) {
    if (p.itemCount == 0) {
        cout << "You have no items." << endl;
        return -1; // exit function
    }

    cout << "\nChoose an item to use:" << endl;
    for (int i = 0; i < p.itemCount; i++) {
        cout << i+1 << ". " << p.inventory[i].name << " (Durability: " << p.inventory[i].durability << "%)" << (p.inventory[i].isBroken() ? " [BROKEN]" : "") << endl;
    }

    cout << "Item: ";
    int choice;
    cin >> choice;

    choice -= 1;
    if (choice < 0 || choice >= p.itemCount) return -1;
    if (p.inventory[choice].isBroken()) return -1;

    return choice;
}
// Actions
void hunt(Player &p) {
    cout << "> You prepare to hunt..." << endl;

    int itemIndex = chooseItem(p);

    int foodGained = 20;
    bool protectedFromAnimal = false;

    if (itemIndex != -1) {
        Item &it = p.inventory[itemIndex];

        foodGained += it.huntBoost;
        protectedFromAnimal = it.protectsFromAnimals;

        useItem(it);  // durability loss
    }

    // 20% chance of animal attack (unless protected)
    if (randomInt(1, 100) <= 20 && !protectedFromAnimal) {
        cout << "A wild animal attacked you! -20 health" << endl;
        p.health -= 20;
    } else if (protectedFromAnimal) {
        cout << "Your item protected you from an animal attack!" << endl;
    }

    // Successful hunt gives food
    cout << "You hunted successfully and gained +" << foodGained << " Hunger." << endl;

    p.hunger += foodGained;
    p.energy -= 10;
}
void explore(Player &p) {
    cout << "> You prepare to explore..." << endl;

    int itemIndex = chooseItem(p);

    int safety = 0;

    if (itemIndex != -1) {
        Item &it = p.inventory[itemIndex];
        safety = it.exploreSafety;
        useItem(it);
    }

    // Danger event chance (reduced by item)
    int dangerChance = 30 - safety; // base 30%

    int roll = randomInt(1, 100);
    if (roll <= dangerChance) {
        cout << "You encountered danger while exploring! -15 Health" << endl;
        p.health -= 15;
    } else {
        cout << "You explored safely. " << endl;
    }

    p.energy -= 20;
    p.hunger -= 5;
}
void gather(Player &p) {
    cout << "> You gathered resources..." << endl;
    p.energy -= 15;
}

void rest(Player &p) {
    cout << "> You rested..." << endl;
    p.energy += 20;
    p.hunger += 10;
}
// Random events
void randomEvent(Player &p) {
    int roll = randomInt(1, 170);

    cout << "--- Random Event ---" << endl;

    if (roll <= 25) {
    bool protect = false;

    // Check if any item protects you
    for (int i = 0; i < p.itemCount; i++) {
        if (p.inventory[i].protectsFromAnimals &&
            !p.inventory[i].isBroken()) {
            protect = true;
            break;
        }
    }

    if (protect) {
        cout << "A wild animal tried to attack, but your gear scared it away!" << endl;
    } else {
        cout << "A wild animal attacked you! -30 Health" << endl;
        p.health -= 30;
    }
}
// 10% Chance - Storm
else if (roll <= 35) {
    cout << "A sudden storm soaked you! -15 Energy " << endl;
    p.energy -= 15;
}

// 10% Chance - Injury
else if (roll <= 45) {
    cout << "You tripped on a rock and got injured! -20 Health" << endl;
    p.health -= 20;
}

// 8% Chance - Poisonous Plant
else if (roll <= 53) {
    cout << "A poisonous plant made you sick! -15 Hunger" << endl;
    p.hunger -= 15;
}

// 7% Chance - Bad Sleep
else if (roll <= 60) {
    cout << "You slept poorly last night. -10 Energy" << endl;
    p.energy -= 10;
}

// 10% Chance - Fresh Berries
else if (roll <= 70) {
    cout << "You found fresh berries! +20 Hunger" << endl;
    p.hunger += 20;
}

// 8% Chance - Stream Found
else if (roll <= 78) {
    cout << "You discovered a small stream! +15 Energy" << endl;
    p.energy += 15;
}

// 8% Chance - Abandoned Camp
else if (roll <= 86) {
    cout << "You found an abandoned camp with supplies! +15 Hunger, +15 Energy" << endl;
    p.hunger += 15;
    p.energy += 15;
}

// 7% Chance - Mushrooms
else if (roll <= 93) {
    cout << "You found edible mushrooms! +10 Hunger" << endl;
    p.hunger += 10;
}
// 7% - Snake Bite
else if (roll <= 100) {
    cout << "A snake bit your leg! -25 Health" << endl;
    p.health -= 25;
}

// 7% - Poisonous insect
else if (roll <= 107) {
    cout << "A poisonous insect stung you! -10 Health" << endl;
    p.health -= 10;
}

// 8% - Falling branch
else if (roll <= 115) {
    cout << "A heavy branch fell and struck you! -20 Health" << endl;
    p.health -= 20;
}

// 7% - Pitfall
else if (roll <= 122) {
    cout << "You fell into a hidden pit! -30 Health" << endl;
    p.health -= 30;
}

// 7% - Heatstroke
else if (roll <= 129) {
    cout << "You suffered mild heatstroke! -10 Health, -10 Energy" << endl;
    p.health -= 10;
    p.energy -= 10;
}

// 8% - Hypothermia
else if (roll <= 137) {
    cout << "The chilling cold weakens your body. -15 Health" << endl;
    p.health -= 15;
}

// 7% - Thorn bushes
else if (roll <= 144) {
    cout << "Thorny bushes cut your arms and legs. -5 Health" << endl;
    p.health -= 5;
}

// 9% - Rockslide
else if (roll <= 153) {
    cout << "A rockslide injured you! -35 Health" << endl;
    p.health -= 35;
}

// 5% - Old trap injury
else if (roll <= 158) {
    cout << "You stepped into an old hunting trap! -25 Health" << endl;
    p.health -= 25;
}

// 2% - Lightning knockdown
else if (roll <= 160) {
    cout << "Lightning struck nearby and knocked you down! -10 Health" << endl;
    p.health -= 10;
}

// 10% - Good Omen
else {
    cout << "A good omen! You feel motivated. +10 Health" << endl;
    p.health += 10;
}
     // Clamp stats to safe limits if a stat exceeds threshold reset to max
    if (p.health > 100) p.health = 100;
    if (p.hunger > 100) p.hunger = 100;
    if (p.energy > 100) p.energy = 100;

    cout << "----------------------" << endl;
}
// Random formula so i can use it whenever
int randomInt(int min, int max) {
    return rand() % (max - min + 1) + min;
}

