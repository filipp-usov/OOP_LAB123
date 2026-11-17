#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "Base.h"
#include "GameField.h"

class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
};

class CreateUnitCommand : public Command {
    Base* base; GameField* field; std::string type; int x, y;
public:
    CreateUnitCommand(Base* b, GameField* f, const std::string& t, int x, int y) 
        : base(b), field(f), type(t), x(x), y(y) {}
    void execute() override {
        base->createUnit(type, x, y);
        Unit* newUnit = nullptr;
        for (int i = 0; i < 10; ++i) {
            Unit* u = base->getUnit(i);
            if (u && u->getX() == x && u->getY() == y) { newUnit = u; break; }
        }
        if (newUnit) field->addUnit(newUnit);
    }
};

class MoveUnitCommand : public Command {
    Base* base; GameField* field; int id; char dir; int steps;
public:
    MoveUnitCommand(Base* b, GameField* f, int id, char d, int s) : base(b), field(f), id(id), dir(d), steps(s) {}
    void execute() override;
};

class AttackCommand : public Command {
    Base* base; int attackerId; int targetId;
public:
    AttackCommand(Base* b, int a, int t) : base(b), attackerId(a), targetId(t) {}
    void execute() override;
};

class CollectCommand : public Command {
    Base* base; int unitId;
public:
    CollectCommand(Base* b, int id) : base(b), unitId(id) {}
    void execute() override;
};

class HealCommand : public Command {
    Base* base; int unitId;
public:
    HealCommand(Base* b, int id) : base(b), unitId(id) {}
    void execute() override;
};

class BuildCommand : public Command {
    Base* base; std::string type; int x, y;
public:
    BuildCommand(Base* b, const std::string& t, int x, int y) : base(b), type(t), x(x), y(y) {}
    void execute() override { base->buildBuilding(type, x, y); }
};


class ProduceCommand : public Command {
    Base* base; GameField* field; std::string type; int buildingId;
public:
    ProduceCommand(Base* b, GameField* f, const std::string& t, int id) : base(b), field(f), type(t), buildingId(id) {}
    void execute() override {
        base->produceUnit(type, buildingId);
        // Добавляем на поле
        for (int i = 0; i < 10; ++i) {
            Unit* u = base->getUnit(i);
            if (u) field->addUnit(u);
        }
    }
};

class ResearchCommand : public Command {
    Base* base; std::string tech;
public:
    ResearchCommand(Base* b, const std::string& t) : base(b), tech(t) {}
    void execute() override { base->startResearch(tech); }
};

class DistributeCommand : public Command {
    Base* base; std::string res; int amount;
public:
    DistributeCommand(Base* b, const std::string& r, int a) : base(b), res(r), amount(a) {}
    void execute() override { base->distributeResources(res, amount); }
};

class StatsCommand : public Command {
    Base* base;
public:
    StatsCommand(Base* b) : base(b) {}
    void execute() override { base->printStats(); }
};