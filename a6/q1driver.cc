#include "q1classes.h"
#include "config.h"
#include <unistd.h>

using std::vector;

void usage()
{
  exit(-1);
}

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
    for (int i = 0; i < parms.numStudents; i++)
    {
      students.push_back(new Student(prt, nameServer, wtcrdOffice,i, parms.maxPurchases));
    }
    
  }
}
