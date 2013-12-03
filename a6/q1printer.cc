#include <uC++.h>
#include <iostream>
#include <cassert>
#include "q1classes.h"

using std::cout;
using std::endl;

    /**
     * Printer::(constructor) 
     * store the arguments, print the first two lines;
     * all data will be recorded in the maps
     */
    Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers ):
        numStudents(numStudents), numVendingMachines(numVendingMachines), numCouriers(numCouriers){

        cout<<"Parent\tWATOff\tNames\tTruck\tPlant\t";
        for (unsigned int i = 0; i<numStudents; i++) {
            cout<<"Stud"<<i<<"\t";
        }
        for (unsigned int i = 0; i<numVendingMachines; i++) {
            cout<<"Mach"<<i<<"\t";
        }
        for (unsigned int i = 0; i<numCouriers; i++) {
            cout<<"Cour"<<i<<"\t";
        }
        cout<<endl;
        int total = 5+numCouriers+numStudents+numVendingMachines;
        for (int i = 0; i<total; i++) {
            cout<<"*******\t";
        }
        cout<<endl;
    }

    /**
     * Printer: print
     * @param kind      type of task having update in state
     * @param state     state letter
     * 
     * If there were records in sGeneral map, flush
     * If the state is Finished 'F', flush finish line
     * record the states in sGeneral if the new state is not 'F'
     */


    void Printer::print( Kind kind, char state ){
        //Ensure only legal tasks are calling this method
        assert(kind!=Student);
        assert(kind!=Vending);
        assert(kind!=Courier);
        if ( sGeneral.count(kind) || state == 'F') {
            flush();
            if (state == 'F') flushFinish(kind, -1);
            else sGeneral.insert(std::pair<Kind, char>(kind, state));
        } else {
            sGeneral.insert(std::pair<Kind, char>(kind, state));
        }

    }

    void Printer::print( Kind kind, unsigned int lid, char state ){
        //Ensure only legal tasks are calling this method
        assert(kind!=Parent);
        assert(kind!=WATCardOffice);
        assert(kind!=NameServer);
        assert(kind!=Truck);
        assert(kind!=BottlingPlant);
        if (kind == Student){
            if ( sStudent.count(lid) == 0 && state != 'F') {
                sStudent.insert(std::pair<int, char>(lid, state));
                return;
            } 
            flush();
            if (state != 'F') {
                sStudent.insert(std::pair<int, char>(lid, state));
            }
        } else if (kind == Vending){
            if ( sStudent.count(lid) == 0 && state != 'F') {
                sVending.insert(std::pair<int, char>(lid, state));
                return;
            }
            flush();
            if (state != 'F') {
                sVending.insert(std::pair<int, char>(lid, state));
            }
        } else if (kind == Courier) {
            if ( sStudent.count(lid) == 0 && state != 'F') {
                sCourier.insert(std::pair<int, char>(lid, state));
                return;
            }
            flush();
            if (state != 'F') {
                sCourier.insert(std::pair<int, char>(lid, state));
            }
        } else {
            // Should never occur, terminate 
            assert(false);
        }
        if (state == 'F'){
            flushFinish(kind, lid);
        }
    }

    void Printer::print( Kind kind, char state, int value1){
        print(kind, state);
        generalNumber1.insert(std::pair<Kind, int>(kind, value1));
    }

    void Printer::print( Kind kind, char state, int value1, int value2){
        print(kind, state, value1);
        generalNumber2.insert(std::pair<Kind, int>(kind, value2));
    }

    void Printer::print( Kind kind, unsigned int lid, char state, int value1){
        print(kind, lid, state);
        if (kind == Student) {
            studentNumber1.insert(std::pair<int, int>(lid, value1));
        } else if (kind == Vending) {
            vendingNumber1.insert(std::pair<int, int>(lid, value1));
        } else if (kind == Courier) {
            courierNumber1.insert(std::pair<int, int>(lid, value1));
        }
    }

    void Printer::print( Kind kind, unsigned int lid, char state, int value1, int value2){
        print(kind, lid, state, value1);
        if (kind == Student) {
            studentNumber2.insert(std::pair<int, int>(lid, value2));
        } else if (kind == Vending) {
            vendingNumber2.insert(std::pair<int, int>(lid, value2));
        } else if (kind == Courier) {
            courierNumber2.insert(std::pair<int, int>(lid, value2));
        }
    }

    /**
     * Flush the status and numbers recorded.
     * clear all maps.
     */
    void Printer::flush(){
        if (sGeneral.count(Parent)) {
            cout<<sGeneral[Parent];
            if (generalNumber1.count(Parent)) {
                cout<<generalNumber1[Parent];
            }
            if (generalNumber2.count(Parent)) {
                cout<<','<<generalNumber2[Parent];
            }
        }
        cout<<"\t";
        if (sGeneral.count(WATCardOffice)) {
            cout<<sGeneral[WATCardOffice];
            if (generalNumber1.count(WATCardOffice)) {
                cout<<generalNumber1[WATCardOffice];
            }
            if (generalNumber2.count(WATCardOffice)) {
                cout<<','<<generalNumber2[WATCardOffice];
            }
        }
        cout<<"\t";
        if (sGeneral.count(NameServer)) {
            cout<<sGeneral[NameServer];
            if (generalNumber1.count(NameServer)) {
                cout<<generalNumber1[NameServer];
            }
            if (generalNumber2.count(NameServer)) {
                cout<<','<<generalNumber2[NameServer];
            }
        }
        cout<<"\t";
        if (sGeneral.count(Truck)) {
            cout<<sGeneral[Truck];
            if (generalNumber1.count(Truck)) {
                cout<<generalNumber1[Truck];
            }
            if (generalNumber2.count(Truck)) {
                cout<<','<<generalNumber2[Truck];
            }
        }
        cout<<"\t";
        if (sGeneral.count(BottlingPlant)) {
            cout<<sGeneral[BottlingPlant];
            if (generalNumber1.count(BottlingPlant)) {
                cout<<generalNumber1[BottlingPlant];
            }
            if (generalNumber2.count(BottlingPlant)) {
                cout<<','<<generalNumber2[BottlingPlant];
            }
        }
        cout<<"\t";
        for (unsigned int i = 0; i < numStudents; i++) {
            if (sStudent.count(i)) {
                cout<<sStudent[i];
                if (studentNumber1.count(i)) {
                    cout<<studentNumber1[i];
                }
                if (studentNumber2.count(i)) {
                    cout<<','<<studentNumber2[i];
                }
            }
            cout<<"\t";
        }
        for (unsigned int i = 0; i < numVendingMachines; i++) {
            if (sVending.count(i)) {
                cout<<sVending[i];
                if (vendingNumber1.count(i)) {
                    cout<<vendingNumber1[i];
                }
                if (vendingNumber2.count(i)) {
                    cout<<','<<vendingNumber2[i];
                }
            }
            cout<<"\t";
        }
        for (unsigned int i = 0; i < numCouriers; i++) {
            if (sCourier.count(i)) {
                cout<<sCourier[i];
                if (courierNumber1.count(i)) {
                    cout<<courierNumber1[i];
                }
                if (courierNumber2.count(i)) {
                    cout<<','<<courierNumber2[i];
                }
            }
            cout<<"\t";
        }
        cout<<endl;
        studentNumber1.clear();
        studentNumber2.clear();
        vendingNumber1.clear();
        vendingNumber2.clear();
        courierNumber1.clear();
        courierNumber2.clear();
        generalNumber1.clear();
        generalNumber2.clear();
        sStudent.clear();
        sVending.clear();
        sCourier.clear();
        sGeneral.clear();
    }

    /**
     * Use to flush for Finished status
     */
    void Printer::flushFinish(Kind kind, unsigned int lid){
        int id = -1;
        switch(kind){
            case Parent: id = 0; break;
            case WATCardOffice: id = 1; break;
            case NameServer: id = 2; break;
            case Truck: id = 3; break;
            case BottlingPlant: id = 4; break;
            case Student: id = 5+lid; break;
            case Vending: id = 5+numStudents+lid; break;
            case Courier: id = 5+numStudents+numVendingMachines+lid; break;
        }

        for (unsigned int i = 0; i<5+numStudents+numVendingMachines+numCouriers; i++) {
          if (i == id) {
            cout<<'F'<<"\t";
          } else {
            cout<<"...\t";
          }
        }
        cout<<endl;
    }


    Printer::~Printer(){
        cout<<"***********************"<<endl;
    }
