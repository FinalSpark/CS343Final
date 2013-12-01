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
  parms = {2, 2, 8, 3, 5, 3, 3, 2, 1}; //default params
  
  //TODO add file reading and parsing

  {

    Bank bank(parms.numStudents);
    Printer prt(parms.numStudents, parms.numVendingMachines, parms.numCouriers);
    NameServer nameServer(prt, parms.numVendingMachines, parms.numStudents);
    WATCardOffice wtcrdOffice(prt,bank,parms.numCouriers);
    BottlingPlant plant(prt, nameServer, parms.numVendingMachines, parms.maxShippedPerFlavour, parms.maxStockPerFlavour, parms.timeBetweenShipments);
    Truck truck(prt,nameServer,plant,parms.numVendingMachines, parms.maxStockPerFlavour);
    for (int i = 0; i < parms.numVendingMachines; i++)
    {
      machines.push_back(new VendingMachine(prt, nameServer, i, parms.sodaCost, parms.maxStockPerFlavour));
    }

    for (int i = 0; i < parms.numStudents; i++)
    {
      students.push_back(new Student(prt, nameServer, wtcrdOffice,i, parms.maxPurchases));
    }
    
  }
}
