#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include "Command.h"

class CommandHandler {
protected:
    std::shared_ptr<CommandHandler> nextHandler;
public:
    void setNext(std::shared_ptr<CommandHandler> handler) { nextHandler = handler; }
    virtual bool handle(const std::string& input, Base& base, GameField& field) = 0;
};

class CreateHandler : public CommandHandler {
public:
    bool handle(const std::string& input, Base& base, GameField& field) override {
        std::istringstream iss(input); 
        std::string cmd, unit, type, at;
        int x, y;
        if (iss >> cmd >> unit >> type >> at >> x >> y && cmd == "create" && unit == "unit" && at == "at") {
            if (type == "1") type = "Swordsman";
            else if (type == "2") type = "Spearman";
            else if (type == "3") type = "LightArcher";
            else if (type == "4") type = "HeavyArcher";
            else if (type == "5") type = "Cataphract";
            else if (type == "6") type = "HorseArcher";
            
            std::cout << "[Chain] Create command parsed.\n";
            CreateUnitCommand(&base, &field, type, x, y).execute();
            return true;
        }
        if (nextHandler) return nextHandler->handle(input, base, field);
        return false;
    }
};

class MoveHandler : public CommandHandler {
public:
    bool handle(const std::string& input, Base& base, GameField& field) override {
        std::istringstream iss(input); std::string cmd, unit;
        int id, steps; char dir;
        if (iss >> cmd >> unit >> id >> dir >> steps && cmd == "move" && unit == "unit") {
            std::cout << "[Chain] Move command parsed.\n";
            MoveUnitCommand(&base, &field, id, dir, steps).execute();
            return true;
        }
        if (nextHandler) return nextHandler->handle(input, base, field);
        return false;
    }
};

class BuildHandler : public CommandHandler {
public:
    bool handle(const std::string& input, Base& base, GameField& field) override {
        std::istringstream iss(input); std::string cmd, type, at;
        int x, y;
        if (iss >> cmd >> type >> at >> x >> y && cmd == "build" && at == "at") {
            std::cout << "[Chain] Build command parsed.\n";
            BuildCommand(&base, type, x, y).execute();
            return true;
        }
        if (nextHandler) return nextHandler->handle(input, base, field);
        return false;
    }
};

class ProduceHandler : public CommandHandler {
public:
    bool handle(const std::string& input, Base& base, GameField& field) override {
        std::istringstream iss(input); 
        std::string cmd, unit, type, in; 
        int id;
        // Парсим: produce unit <type> in <id>
        if (iss >> cmd >> unit >> type >> in >> id && cmd == "produce" && unit == "unit" && in == "in") {
            
            if (type == "1") type = "Swordsman";
            else if (type == "2") type = "Spearman";
            else if (type == "3") type = "LightArcher";
            else if (type == "4") type = "HeavyArcher";
            else if (type == "5") type = "Cataphract";
            else if (type == "6") type = "HorseArcher";

            std::cout << "[Chain] Produce command parsed.\n";
            ProduceCommand(&base, &field, type, id).execute();
            return true;
        }
        if (nextHandler) return nextHandler->handle(input, base, field);
        return false;
    }
};

class MiscHandler : public CommandHandler {
public:
    bool handle(const std::string& input, Base& base, GameField& field) override {
        std::istringstream iss(input); std::string cmd;
        iss >> cmd;
        
        if (cmd == "attack") {
            std::string unit; int a, t; 
            if (iss >> unit >> a >> t) { AttackCommand(&base, a, t).execute(); return true; }
        }
        if (cmd == "collect") {
            std::string unit; int id; 
            if (iss >> unit >> id) { CollectCommand(&base, id).execute(); return true; }
        }
        if (cmd == "heal") {
            std::string unit; int id; 
            if (iss >> unit >> id) { HealCommand(&base, id).execute(); return true; }
        }
        if (cmd == "research") {
            std::string tech; 
            if (iss >> tech) { ResearchCommand(&base, tech).execute(); return true; }
        }
        if (cmd == "distribute") {
            std::string res; int amt; 
            if (iss >> res >> amt) { DistributeCommand(&base, res, amt).execute(); return true; }
        }
        if (cmd == "stats") {
            StatsCommand(&base).execute(); return true;
        }
        if (nextHandler) return nextHandler->handle(input, base, field);
        return false;
    }
};