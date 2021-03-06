#include <uC++.h>
#include "q1classes.h"
#include "MPRNG.h"
#include <uFuture.h>
#include <iostream>
using namespace std;
MPRNG ran;

/**
 student tries to purchase drink.
 If card is lost, exception is thrown, new card will be created.
  (works as described in assignment)
*/
    void Student::main(){
        int purchases = ran(1, maxPurchases);
        VendingMachine::Flavours favFlavour =  static_cast<VendingMachine::Flavours>(ran(0, 3));
        prt->print( Printer::Student, id, 'S', favFlavour, purchases);
        WATCard::FWATCard card = cardOffice->create(id, 5);
        VendingMachine *vm = nameServer->getMachine(id);
        prt->print( Printer::Student, id, 'V', vm->getId());
        while(purchases) {
            int times = ran(1,10);
            yield(times);
            bool watCardLost;
            VendingMachine::Status status;
            unsigned int balance;

            do {
                try {
                    status = vm->buy(favFlavour, *(card()));
                    watCardLost = false;
                } catch (WATCardOffice::Lost& e) {
                    watCardLost = true;
                    prt->print( Printer::Student, id, 'L');
                    card = cardOffice->create(id, 5);
                }
            } while (watCardLost);

            switch (status) {
                case VendingMachine::BUY:
                    purchases--;
                    prt->print( Printer::Student, id, 'B', card()->getBalance() );
                    break;
                case VendingMachine::STOCK:
                    vm = nameServer->getMachine(id);
                    break;
                case VendingMachine::FUNDS:
                    cardOffice->transfer(id, 5+vm->cost(), card());

                    break;
                default:
                    break;
            }
        }
        prt->print( Printer::Student, id, 'F');
        delete card;
    }

    Student::Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id, unsigned int maxPurchases ):
        prt(&prt),nameServer(&nameServer),cardOffice(&cardOffice),id(id), maxPurchases(maxPurchases){

        } 
