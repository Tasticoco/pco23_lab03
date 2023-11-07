#include "extractor.h"
#include "costs.h"
#include <pcosynchro/pcothread.h>

WindowInterface* Extractor::interface = nullptr;

Extractor::Extractor(int uniqueId, int fund, ItemType resourceExtracted)
    : Seller(fund, uniqueId), resourceExtracted(resourceExtracted), nbExtracted(0)
{
    assert(resourceExtracted == ItemType::Copper ||
           resourceExtracted == ItemType::Sand ||
           resourceExtracted == ItemType::Petrol);
    interface->consoleAppendText(uniqueId, QString("Mine Created"));
    interface->updateFund(uniqueId, fund);
}

std::map<ItemType, int> Extractor::getItemsForSale() {
    return stocks;
}

int Extractor::trade(ItemType it, int qty) {
    // TODO
    mutex.lock();
    auto iter  = stocks.find(it); //Pour ne pas avoir de "out of range" plus tard

    if(qty <= 0 || iter == stocks.end() || stocks.at(it) < qty){
        mutex.unlock();
        return 0;
    }
    int costTrade = getCostPerUnit(it) * qty;
    money += costTrade;
    stocks.at(it) -= qty;
    mutex.unlock();
    interface->consoleAppendText(uniqueId, QString("I just sold %1").arg(qty) % QString(" of %1").arg(getItemName(it)));
    return costTrade;
}

void Extractor::run() {
    interface->consoleAppendText(uniqueId, "[START] Mine routine");

    while (!PcoThread::thisThread()->stopRequested()) {
        /* TODO concurrence */


        int minerCost = getEmployeeSalary(getEmployeeThatProduces(resourceExtracted));

        mutex.lock();

        if (money < minerCost) {
            /* Pas assez d'argent */
            /* Attend des jours meilleurs */
            mutex.unlock();
            PcoThread::usleep(1000U);
            continue;
        }

        /* On peut payer un mineur */
        money -= minerCost;
        mutex.unlock();
        /* Temps aléatoire borné qui simule le mineur qui mine */
        PcoThread::usleep((rand() % 100 + 1) * 10000);
        mutex.lock();
        /* Statistiques */
        nbExtracted++;
        /* Incrément des stocks */
        stocks[resourceExtracted] += 1;

        mutex.unlock();
        /* Message dans l'interface graphique */
        interface->consoleAppendText(uniqueId, QString("1 ") % getItemName(resourceExtracted) %
                                     " has been mined");
        /* Update de l'interface graphique */
        interface->updateFund(uniqueId, money);
        interface->updateStock(uniqueId, &stocks);
    }
    interface->consoleAppendText(uniqueId, "[STOP] Mine routine");
}

int Extractor::getMaterialCost() {
    return getCostPerUnit(resourceExtracted);
}

ItemType Extractor::getResourceMined() {
    return resourceExtracted;
}

int Extractor::getAmountPaidToMiners() {
    return nbExtracted * getEmployeeSalary(getEmployeeThatProduces(resourceExtracted));
}

void Extractor::setInterface(WindowInterface *windowInterface) {
    interface = windowInterface;
}

SandExtractor::SandExtractor(int uniqueId, int fund): Extractor::Extractor(uniqueId, fund, ItemType::Sand) {}

CopperExtractor::CopperExtractor(int uniqueId, int fund): Extractor::Extractor(uniqueId, fund, ItemType::Copper) {}

PetrolExtractor::PetrolExtractor(int uniqueId, int fund): Extractor::Extractor(uniqueId, fund, ItemType::Petrol) {}
