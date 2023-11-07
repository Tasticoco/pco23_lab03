#include "factory.h"
#include "extractor.h"
#include "costs.h"
#include "wholesale.h"
#include <pcosynchro/pcothread.h>
#include <iostream>
#include <algorithm>
#include <random>

WindowInterface* Factory::interface = nullptr;


Factory::Factory(int uniqueId, int fund, ItemType builtItem, std::vector<ItemType> resourcesNeeded)
    : Seller(fund, uniqueId), resourcesNeeded(resourcesNeeded), itemBuilt(builtItem), nbBuild(0)
{
    assert(builtItem == ItemType::Chip ||
           builtItem == ItemType::Plastic ||
           builtItem == ItemType::Robot);

    interface->updateFund(uniqueId, fund);
    interface->consoleAppendText(uniqueId, "Factory created");
}

void Factory::setWholesalers(std::vector<Wholesale *> wholesalers) {
    Factory::wholesalers = wholesalers;

    for(Seller* seller: wholesalers){
        interface->setLink(uniqueId, seller->getUniqueId());
    }
}

ItemType Factory::getItemBuilt() {
    return itemBuilt;
}

int Factory::getMaterialCost() {
    return getCostPerUnit(itemBuilt);
}

bool Factory::verifyResources() {
    for (auto item : resourcesNeeded) {
        if (stocks[item] == 0) {
            return false;
        }
    }

    return true;
}

void Factory::buildItem() {

    // TODO
    int buildCost = getEmployeeSalary(getEmployeeThatProduces(getItemBuilt()));

    mutex.lock();
    if(money < buildCost){
        mutex.unlock();
        PcoThread::usleep(1000U);
        //On attend de meilleurs jours
    } else {        
        money -= buildCost;
        //Temps simulant l'assemblage d'un objet.
        PcoThread::usleep((rand() % 100) * 100000);

        // TODO
        for(auto itUsed : resourcesNeeded){
            stocks.at(itUsed) -= 1;
        }

        ++nbBuild;
        stocks.at(getItemBuilt()) += 1;
        mutex.unlock();
        interface->consoleAppendText(uniqueId, "Factory have build a new object");
    }

}

void Factory::orderResources() {

    // TODO - Itérer sur les resourcesNeeded et les wholesalers disponibles
    auto rng = std::default_random_engine{};
    auto randWholesale(wholesalers);
    ItemType item = resourcesNeeded.at(0);

    mutex.lock();

    for(ItemType itemComp : resourcesNeeded){
        auto iter  = stocks.find(itemComp);
        if(iter == stocks.end()){
            continue;
        }
        item = (stocks.at(itemComp) < stocks.at(item) ? itemComp : item);
    }



    std::shuffle(std::begin(randWholesale), std::end(randWholesale), rng);
    for(auto wholesale : randWholesale){
        if(money >= getCostPerUnit(item)){
            if (wholesale->trade(item,1)){
                money -= getCostPerUnit(item);
                stocks.at(item) += 1;
                break;
            }
        }
    }

    mutex.unlock();

    //Temps de pause pour éviter trop de demande
    PcoThread::usleep(10 * 100000);

}

void Factory::run() {
    if (wholesalers.empty()) {
        std::cerr << "You have to give to factories wholesalers to sales their resources" << std::endl;
        return;
    }
    interface->consoleAppendText(uniqueId, "[START] Factory routine");

    while (!PcoThread::thisThread()->stopRequested()) {
        if (verifyResources()) {
            buildItem();
        } else {
            orderResources();
        }
        interface->updateFund(uniqueId, money);
        interface->updateStock(uniqueId, &stocks);
    }
    interface->consoleAppendText(uniqueId, "[STOP] Factory routine");
}

std::map<ItemType, int> Factory::getItemsForSale() {
    return std::map<ItemType, int>({{itemBuilt, stocks[itemBuilt]}});
}

int Factory::trade(ItemType it, int qty) {
    mutex.lock();
    auto iter  = stocks.find(it);
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

int Factory::getAmountPaidToWorkers() {
    return Factory::nbBuild * getEmployeeSalary(getEmployeeThatProduces(itemBuilt));
}

void Factory::setInterface(WindowInterface *windowInterface) {
    interface = windowInterface;
}

PlasticFactory::PlasticFactory(int uniqueId, int fund) :
    Factory::Factory(uniqueId, fund, ItemType::Plastic, {ItemType::Petrol}) {}

ChipFactory::ChipFactory(int uniqueId, int fund) :
    Factory::Factory(uniqueId, fund, ItemType::Chip, {ItemType::Sand, ItemType::Copper}) {}

RobotFactory::RobotFactory(int uniqueId, int fund) :
    Factory::Factory(uniqueId, fund, ItemType::Robot, {ItemType::Chip, ItemType::Plastic}) {}
