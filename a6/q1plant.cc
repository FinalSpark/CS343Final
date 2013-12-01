#include <uC++.h>
#include "q1classes.h"
#include "MPRNG.h"

    BottlingPlant::BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                 unsigned int timeBetweenShipments ):
        prt(&prt), nameServer(&nameServer), numVendingMachines(numVendingMachines),
        maxShippedPerFlavour(maxShippedPerFlavour), maxStockPerFlavour(maxStockPerFlavour),
        timeBetweenShipments(timeBetweenShipments){

        }

    bool BottlingPlant::getShipment( unsigned int cargo[] ){
        if (closed) return true;
        for (int i = 0; i < 4; i++ ){
            cargo[i] = soda[i];
        }
        return false;

    }

    void BottlingPlant::main(){
        prt->print(Plant, 'S');
        Truck truck = new Truck(prt, nameServer, *this, numVendingMachines, maxStockPerFlavour);
        while (true) {
            for (int i = 0; i < 4; i++ ){
                soda[i] = ran(0, maxShippedPerFlavour);
            }
            prt->print(Plant, 'G', soda[0]+soda[1]+soda[2]+soda[3]);
            _Accept(~BottlingPlant) {
                closed = true;
                prt->print(Plant, 'F');
                break;
            } or _Accept(getShipment) {
                prt->print(Plant, 'P');
                yield(timeBetweenShipments);
            }
        }
    }

    BottlingPlant::~BottlingPlant(){
        delete truck;
    }