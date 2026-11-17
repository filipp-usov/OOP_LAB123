#pragma once
#include <memory>
#include "Base.h"
#include "GameField.h"
#include "Handlers.h"

class GameFacade {
private:
    Base* base;
    GameField* field;
    std::shared_ptr<CommandHandler> chainStart;

    void setupChain() {
        auto createH = std::make_shared<CreateHandler>();
        auto moveH = std::make_shared<MoveHandler>();
        auto buildH = std::make_shared<BuildHandler>();
        auto produceH = std::make_shared<ProduceHandler>(); 
        auto miscH = std::make_shared<MiscHandler>();

        createH->setNext(moveH);
        moveH->setNext(buildH);
        buildH->setNext(produceH); 
        produceH->setNext(miscH);
        chainStart = createH;
    }

public:
    GameFacade(Base* b, GameField* f) : base(b), field(f) { setupChain(); }

    void processInput(const std::string& input) {
        if (input == "end_turn") {
            base->tick();
            return;
        }
        if (!chainStart->handle(input, *base, *field)) {
            std::cout << "Unknown command.\n";
        }
    }
};