#include "q1classes.h"
#include "config.h"
#include <unistd.h>

using std::vector;

void usage()
{
  exit(-1);
}

vector <VendingMachine *> machines;
vector <Student *> students;
void uMain::main() {
  struct ConfigParms parms;
  parms = {2, 4, 10, 5, 10, 4, 7, 10, 1}; //default params
  
  //TODO add file reading and parsing

  {

    Bank bank(parms.numStudents);
    Printer prt(parms.numStudents, parms.numVendingMachines, parms.numCouriers);
    NameServer* nameServer = new NameServer(prt, parms.numVendingMachines, parms.numStudents);
    WATCardOffice* wtcrdOffice = new WATCardOffice(prt,bank,parms.numCouriers);
    BottlingPlant* plant = new BottlingPlant(prt, *nameServer, parms.numVendingMachines, parms.maxShippedPerFlavour, parms.maxStockPerFlavour, parms.timeBetweenShipments);
    Parent *parent = new Parent(prt, bank, parms.numStudents, parms.parentalDelay );
    for (unsigned int i = 0; i < parms.numVendingMachines; i++)
    {
      machines.push_back(new VendingMachine(prt, *nameServer, i, parms.sodaCost, parms.maxStockPerFlavour));
    }

    for (unsigned int i = 0; i < parms.numStudents; i++)
    {
      students.push_back(new Student(prt, *nameServer, *wtcrdOffice,i, parms.maxPurchases));
    }

    for (unsigned int i = 0; i < parms.numStudents; i++)
    {
      delete students[i];
    }
    delete plant;
    delete wtcrdOffice;
    delete nameServer;
    delete parent;
    for (unsigned int i = 0; i < parms.numVendingMachines; i++)
    {
      delete machines[i];
    }

  }
}
