#pragma once
#include <iostream>
#include "Base.h"
#include "GameField.h"

class GameMediator {
private:
    Base* base;
    GameField* field;
public:
    GameMediator(Base* b, GameField* f) : base(b), field(f) {}


    void coordinateMovement(int unitId, int newX, int newY) {
        Unit* unit = base->getUnitById(unitId);
        if (!unit) {
            std::cout << "[Mediator] Unit not found!\n";
            return;
        }
        

        TerrainProxy proxy = field->getTerrainProxy(newX, newY);
        if (proxy.isPassable(*unit)) {
            unit->move(newX, newY);
            std::cout << "[Mediator] Unit " << unitId << " moved to (" << newX << ", " << newY << ")\n";
        } else {
            std::cout << "[Mediator] Path blocked!\n";
        }
    }
};