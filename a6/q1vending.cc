#include <uC++.h>
#include "q1classes.h"
#include <iostream>

using std::cout;
using std::endl;
void VendingMachine::main(){
    prt->print(Printer::Vending, id, 'S', sodaCost);
    nameServer->VMregister(this);
    while (true) {
        _Accept(~VendingMachine){
            prt->print(Printer::Vending, id, 'F');
            break;
        } or _When (!restocking) _Accept(buy, inventory) {

        } or _When (restocking) _Accept(restocked){
            // prt->print(Printer::Vending, id, 'R');
            restocking = false;
        }
    }
}

VendingMachine::Status VendingMachine::buy(Flavours flavour, WATCard &card){
    buySuccess = false;
    if (stock[flavour] == 0) return STOCK;
    else if (card.getBalance() < sodaCost) return FUNDS;
    else {
        prt->print(Printer::Vending, id, 'B', flavour, stock[flavour]);
        card.withdraw(sodaCost);
        return BUY;
    }
}

unsigned int * VendingMachine::inventory(){
    restocking = true;
    prt->print(Printer::Vending, id, 'r');
    return stock;
}

void VendingMachine::restocked(){
    prt->print(Printer::Vending, id, 'R');
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
