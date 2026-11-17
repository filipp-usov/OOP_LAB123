#include <iostream>
#include <string>
#include <memory>
#include <cstdlib>
#include <ctime>
#include "GameField.h"
#include "Base.h"
#include "UnitFactory.h"
#include "GameFacade.h"

void generateRandomMap(GameField& field, int seed = 42) {
    srand(seed); 
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            int r = rand() % 100; 
            if (r < 20) {
                field.setTerrain(x, y, std::make_shared<Forest>());     
            } else if (r < 35) {
                field.setTerrain(x, y, std::make_shared<Mountains>());  
            } else if (r < 50) {
                field.setTerrain(x, y, std::make_shared<Swamp>());      
            }
            
        }
    }
}

void runLab1() {
    std::cout << "\n LAB 1 SCENARIOS \n";
    GameField field(10, 10, 5); 
    generateRandomMap(field);

    InfantryFactory infFactory;
    ArcherFactory archFactory;
    CavalryFactory cavFactory;

    field.populateField(infFactory, "Swordsman", 0, 1, 1);
    field.populateField(archFactory, "LightArcher", 0, 4, 2);
    field.populateField(cavFactory, "Cataphract", 0, 7, 4);
    field.populateField(infFactory, "Spearman", 0, 2, 5);
    
    std::cout << "\nUnits on field: " << field.size() << "\n";
    field.printField(); 

    std::cout << "\n Scenario 1.2: Deep copy \n";
    GameField copyField = field; 
    field.getUnit(0)->move(5, 5);
    std::cout << "Original: (" << field.getUnit(0)->getX() << ", " << field.getUnit(0)->getY() << ")\n";
    std::cout << "Copy: (" << copyField.getUnit(0)->getX() << ", " << copyField.getUnit(0)->getY() << ")\n";

    std::cout << " Scenario 1.3: Iteration \n";
    for (auto it = field.begin(); it != field.end(); ++it) {
        Unit* u = *it;
        std::cout << "Found: " << u->getType() << " at (" << u->getX() << ", " << u->getY() << ")\n";
    }
}

void runLab2() {
    std::cout << "\n LAB 2 SCENARIOS \n";
    InfantryFactory infFactory;
    Base myBase(5, 5, 1000, 5, 10, 500, &infFactory); 
    myBase.createUnit("Swordsman", 1, 1); 
    myBase.createUnit("Spearman", 2, 2);

    GameField newField; 
    generateRandomMap(newField);
    newField.setTerrain(2, 1, std::make_shared<Plains>());
    newField.setTerrain(3, 1, std::make_shared<Plains>());
    newField.addUnit(myBase.getUnitById(1));
    newField.addUnit(myBase.getUnitById(2));

    newField.printMapWithBase(myBase.getX(), myBase.getY());

    Unit* hero = myBase.getUnit(0); 
    for (int step = 1; step <= 4; ++step) {
        int newX = hero->getX() + 1;
        int newY = hero->getY();
        TerrainProxy proxy = newField.getTerrainProxy(newX, newY);
        if (proxy.isPassable(*hero)) {
            hero->move(newX, newY);
            newField.printMapWithBase(myBase.getX(), myBase.getY());
        } else break; 
    }

    TreasureChest chest; *hero + chest;
    Trap trap; *hero - trap;
    hero->takeDamage(500); 
}

void runLab3() {
    std::cout << "\n LAB 3 SCENARIOS (Interactive Control) \n";
    InfantryFactory infFactory;
    Base myBase(5, 5, 1000, 5, 10, 100, &infFactory); 

    GameField field;
    generateRandomMap(field);
    field.setTerrain(5, 5, std::make_shared<Plains>());

    field.addObject(3, 3, std::make_shared<TreasureChest>());
    field.addObject(7, 7, std::make_shared<Trap>());

    GameFacade facade(&myBase, &field);

     std::cout << "\n GAME CONTROLS \n";
    std::cout << " UNIT COMMANDS \n";
    std::cout << "  create unit <1-6> at <x> <y>  (create unit 1 at 4 4)\n";
    std::cout << "     Types: 1-Swordsman, 2-Spearman, 3-LightArcher\n";
    std::cout << "            4-HeavyArcher, 5-Cataphract, 6-HorseArcher\n";
    std::cout << "  move unit <id> <dir> <steps>  (move unit 1 d 2)\n";
    std::cout << "  attack unit <att> <tgt>       (attack unit 1 2)\n";
    std::cout << "  collect unit <id>             (collect unit 1)\n";
    std::cout << "  heal unit <id>                (heal unit 1)\n";
    std::cout << "\n BASE COMMANDS \n";
    std::cout << "  build <type> at <x> <y>       (build Barracks at 3 3)\n";
     std::cout << "  produce unit <1-6> in <id>  (produce unit 1 in 1)\n";
    std::cout << "     Types: 1-Swordsman, 2-Spearman, 3-LightArcher\n";
    std::cout << "            4-HeavyArcher, 5-Cataphract, 6-HorseArcher\n";
    std::cout << "  research <tech>               (research armor_upgrade)\n";
    std::cout << "  distribute <res> <amount>     (distribute gold 100)\n";
    std::cout << "\n GAME STATE \n";
    std::cout << "  stats                         (show all info)\n";
    std::cout << "  end_turn                      (advance turn)\n";
    std::cout << "  exit                          (back to menu)\n";

    std::string input;
    while (true) {
        field.printMapWithBase(myBase.getX(), myBase.getY(), &myBase);
        std::cout << "\nEnter command: ";
        std::getline(std::cin, input);
        if (input == "exit") break;
        facade.processInput(input);
    }
}

int main() {
    int choice = 0;
    while (true) {
        std::cout << "    MAIN MENU (MMO RPG) \n";
        std::cout << "1. Run Lab 1 scenarios\n";
        std::cout << "2. Run Lab 2 scenarios\n";
        std::cout << "3. Run Lab 3 scenarios (Interactive)\n";
        std::cout << "0. Exit\n";
        std::cout << "Choose an option: ";
        
        std::string choiceStr;
        std::getline(std::cin, choiceStr);
        if (choiceStr.empty()) continue;

        try { choice = std::stoi(choiceStr); } 
        catch (...) { continue; }

        if (choice == 1) runLab1();
        else if (choice == 2) runLab2();
        else if (choice == 3) runLab3();
        else if (choice == 0) break;
    }
    return 0;
}