#include <uC++.h>
#include "q1classes.h"
#include "MPRNG.h"


NameServer::NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents ):
    prt(&prt), numVendingMachines(numVendingMachines), numStudents(numStudents){
        vmList = new VendingMachine*[numVendingMachines];
        counter = 0;
        vmCounter = 0;
        for (int i = 0; i<numStudents; i++) {
            assignment[i] = i%numVendingMachines;
        }
    }

    void NameServer::main(){
        while(vmCounter<numVendingMachines) {
            _Accept(VMregister){
                vmCounter++;
            }
        }
        while (true) {
            _Accept (~NameServer){
            } or _Accept(getMachine) {
                assignment[sid] = (assignment[sid]+1)%numVendingMachines;
            } or _Accept(getMachineList){

            }
        }
    }

    void NameServer::VMregister( VendingMachine *vendingmachine ){
        vmList[vmCounter] = vendingmachine;
    }

    VendingMachine * NameServer::getMachine( unsigned int id ){
        sid = id;
        return vmList[assignment[i]];
    }
    VendingMachine ** NameServer::getMachineList(){
        return vmList;
    }

    NameServer::~NameServer(){
        delete[] vmList;
    }