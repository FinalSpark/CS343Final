#include "q1classes.h"
#include "config.h"
#include <unistd.h>
#include <iostream>
#include <time.h>

using std::cout;
using std::endl;
using std::vector;

void usage()
{

  exit(-1);
}

vector <VendingMachine *> machines;
vector <Student *> students;
void uMain::main() {
  string configFile = "soda.Config";
  time_t randomSeed = time(NULL);
  switch (argc) {
    case 3: randomSeed = atoi(argv[2]);
    case 2: configFile = argv[1];
    case 1: break;
    default: break;
  }
  struct ConfigParms parms;
  processConfigFile(configFile, parms);
  ran.seed(Seed);

  parms = {2, 3, 8, 3, 5, 3, 3, 2, 1}; //default params
  
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
    delete parent;
    delete wtcrdOffice;
    delete nameServer;
    for (unsigned int i = 0; i < parms.numVendingMachines; i++)
    {
      delete machines[i];
    }
  }
}
