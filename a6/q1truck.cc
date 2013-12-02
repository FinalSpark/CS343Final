#include <uC++.h>
#include "q1classes.h"
#include "MPRNG.h"

void Truck::main(){
    VendingMachine ** vmList = nameServer->getMachineList();
    prt->print(Printer::Truck, 'S');
    while(true) {
        int times = ran(1, 10); //Tim Horton's
        yield(times);
        bool closed = plant->getShipment(cargo);
        if (closed) break;
        prt->print(Printer::Truck, 'P', cargo[0]+cargo[1]+cargo[2]+cargo[3]);
        for (unsigned int i = 0; i<numVendingMachines;i++){
            unsigned int* inventory = vmList[i]->inventory();
            int unStocked = 0;
            for (int flavour = 0; flavour < 4; flavour++) {   //stock each flavour;
                int toStock = 0;
                if ( (maxStockPerFlavour - inventory[flavour]) <= cargo[flavour] ) {
                    toStock = (maxStockPerFlavour - inventory[flavour]);
                } else {
                    toStock = cargo[flavour];
                    unStocked += maxStockPerFlavour - toStock;
                }
                inventory[flavour]+=toStock;
                cargo[flavour]-=toStock;
            }
            if (unStocked > 0) {
                prt->print(Printer::Truck, 'U', i, unStocked);
            }
            prt->print(Printer::Truck, 'd', i, cargo[0]+cargo[1]+cargo[2]+cargo[3]);
            vmList[i]->restocked();
            if (!(cargo[0]+cargo[1]+cargo[2]+cargo[3])){
                break;
            }
        }
    }
    prt->print(Printer::Truck, 'F');
}

Truck::Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
    unsigned int numVendingMachines, unsigned int maxStockPerFlavour ):
    prt(&prt), nameServer(&nameServer), plant(&plant), numVendingMachines(numVendingMachines), maxStockPerFlavour(maxStockPerFlavour){
    }