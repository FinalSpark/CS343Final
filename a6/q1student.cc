#include <uC++.h>
#include <q1classes.h>
#include <MPRNG.h>

MPRNG ran;

    void Student::main(){
        int purchases = ran(1, maxPurchases);
        VendingMachine::Flavours favFlavour =  static_cast<Flavours>(ran(0, 3));
        prt->print( Printer::Student, id, 'S', favFlavour, purchases);
        WATCard *card = cardOffice->create(id, 5);
        VendingMachine *vm = nameServer->getMachine(id);
        prt->print( Printer::Student, id, 'V', vm->getId());
        while(true) {
            int times = ran(1,10);
            yield(times);
            while (watCardLost) {
                bool watCardLost = false;
                try {
                    card();
                } catch (Lost& e) {
                    watCardLost = true;
                    prt->print( Printer::Student, id, 'L');
                    card = cardOffice->create(id, 5);
                }
            }
            VendingMaCHINE::Status status = vm->buy(favFlavour, *(card()));
            switch (status) {
                VendingMachine::BUY:
                    purchases--;
                    prt->print( Printer::Student. id, 'B', card->getBalance());
                    break;
                VendingMachine::STOCK:
                    vm = nameServer->getMachine(id);
                    break;
                VendingMachine::FUND:
                    cardOffice->transfer(id, 5+vm->cost(), card);
                    break;
                default:
                    break;
            }
        }
        prt->print( Printer::Student, id, 'F');
    }

    Student::Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id, unsigned int maxPurchases ):
        prt(&prt),nameServer(&nameServer),cardOffice(&cardOffice),id(id), maxPurchases(maxPurchases){

        } 

    Student::~Student(){
        delete card;
    }
