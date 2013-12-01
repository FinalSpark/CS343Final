#include <uC++.h>
#include "q1classes.h"

void VendingMachine::main(){
    prt->print(Printer::Vending, id, 'S', sodaCost);
    while (true) {
        _Accept(~VendingMachine){
            prt->print(Printer::Vending, id, 'F');
            break;
        } or _When (!restocking) _Accept(inventory) { 
            prt->print(Printer::Vending, id, 'r');
            restocking = true;
        } or _When (restocking) _Accept(restocked){
            prt->print(Printer::Vending, id, 'R', sodaCost);
            restocking = false;
        } or _When(!restocking) _Accept(buy) {
            if (buySuccess)
                prt->print(Printer::Vending, id, 'B', buyFlavour, stock[buyFlavour]);
        }
    }
}

VendingMachine::Status VendingMachine::buy(Flavours flavour, WATCard &card){
    buySuccess = false;
    if (stock[flavour] == 0) return STOCK;
    else if (card.getBalance() < sodaCost) return FUNDS;
    else {
        card.withdraw(sodaCost);
        buySuccess = true;
        buyFlavour = flavour; 
        return BUY;
    }
}

unsigned int * VendingMachine::inventory(){
    return stock;
}

void VendingMachine::restocked(){
    return;
}

unsigned int VendingMachine::cost() {
    return sodaCost;
}

unsigned int VendingMachine::getId() {
    return id;
}

VendingMachine::VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
                    unsigned int maxStockPerFlavour ): prt(&prt), nameServer(&nameServer), id(id), sodaCost(sodaCost){
    stock[0] = 0;
    stock[1] = 0;
    stock[2] = 0;
    stock[3] = 0;
    restocking = false;
    buySuccess = false;
}