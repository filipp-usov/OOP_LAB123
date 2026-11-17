#include "Command.h"
#include "GameMediator.h"

void MoveUnitCommand::execute() {
    Unit* unit = base->getUnitById(id);
    if (!unit) { std::cout << "Unit not found!\n"; return; }
    int dx = 0, dy = 0;
    if (dir == 'w' || dir == 'W') dy = -1;
    else if (dir == 's' || dir == 'S') dy = 1;
    else if (dir == 'a' || dir == 'A') dx = -1;
    else if (dir == 'd' || dir == 'D') dx = 1;

    for (int i = 0; i < steps; ++i) {
        GameMediator mediator(base, field);
        mediator.coordinateMovement(id, unit->getX() + dx, unit->getY() + dy);
    }
}

void AttackCommand::execute() {
    Unit* att = base->getUnitById(attackerId);
    Unit* tgt = base->getUnitById(targetId);
    if (att && tgt) {
        int dmg = att->getAttack().getValue();
        std::cout << "[Combat] " << att->getType() << " attacks " << tgt->getType() << " for " << dmg << " dmg!\n";
        tgt->takeDamage(dmg);
    } else {
        std::cout << "[Combat] Invalid attacker or target!\n";
    }
}

void CollectCommand::execute() {
    Unit* unit = base->getUnitById(unitId);
    if (unit) {
        std::cout << "[Collect] " << unit->getType() << " collected resources!\n";
        base->gatherResources(); 
    } else {
        std::cout << "[Collect] Unit not found!\n";
    }
}

void HealCommand::execute() {
    Unit* unit = base->getUnitById(unitId);
    if (unit) {
        std::cout << "[Heal] " << unit->getType() << " healed!\n";
        unit->getHealth().increase(30);
    } else {
        std::cout << "[Heal] Unit not found!\n";
    }
}