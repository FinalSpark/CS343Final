#include <uC++.h>
#include "q1classes.h"
#include "MPRNG.h"

NameServer::NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents ):
    prt(&prt), numVendingMachines(numVendingMachines), numStudents(numStudents){
        vmList = new VendingMachine*[numVendingMachines];
        counter = 0;
        vmCounter = 0;
        assignment = new unsigned int[numStudents];
        for (unsigned int i = 0; i<numStudents; i++) {
            assignment[i] = i%numVendingMachines;
        }
    }

    /**
    * NameServer::main
    * First accept register. After all vending machines are registered, proceed to accept getMachine and getMachineList
    * Not actually doing much, server keeps track of the assignment of all students, and increment the number everytime a student requests to get Machine
    */
    void NameServer::main(){
        prt->print(Printer::NameServer, 'S');
        while(vmCounter<numVendingMachines) {
            _Accept(VMregister){
                vmCounter++;
                prt->print(Printer::NameServer, 'R', vid);

            }
        }
        while (true) {
            _Accept (~NameServer){
                prt->print(Printer::NameServer, 'F');
                break;
            } or _Accept(getMachine) {

                assignment[sid] = (assignment[sid]+1)%numVendingMachines;
            } or _Accept(getMachineList){

            }
        }
    }

    void NameServer::VMregister( VendingMachine *vendingmachine ){
        vid = vendingmachine->getId();
        vmList[vid] = vendingmachine;
    }

    VendingMachine * NameServer::getMachine( unsigned int id ){
        sid = id;
        prt->print(Printer::NameServer, 'N', sid, assignment[sid]);
        return vmList[assignment[id]];
    }
    VendingMachine ** NameServer::getMachineList(){
        return vmList;
    }

    NameServer::~NameServer(){
        delete[] vmList;
        delete[] assignment;
    }
