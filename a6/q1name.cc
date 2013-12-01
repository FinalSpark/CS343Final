#include <uC++.h>
#include "q1classes.h"
#include "MPRNG.h"


NameServer::NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents ):
    prt(&prt), numVendingMachines(numVendingMachines), numStudents(numStudents){
        vmList = new VendingMachine*[numVendingMachines];
        counter = 0;
    }

    void NameServer::main(){
        
    }

    void NameServer::VMregister( VendingMachine *vendingmachine ){

    }

    VendingMachine * NameServer::getMachine( unsigned int id ){
        return vmList[];
    }
    VendingMachine ** NameServer::getMachineList(){
        return vmList;
    }

    NameServer::~NameServer(){
        delete[] vmList;
    }