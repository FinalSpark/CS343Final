#include <uC++.h>
#include "q1classes.h"
#include "MPRNG.h"

void Parent::main(){
    prt->print(Printer::Parent, 'S');
    while (true) {
        _Accept(~Parent) {
            prt->print(Printer::Parent, 'F');
            break;
        } _Else {
            yield(parentalDelay);
            int student = ran(0, numStudents-1);
            int money = ran(1,3);
            bank->deposit(student, money);
            prt->print(Printer::Parent, 'D', student, money);
        }
    }    
}

Parent::Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay ):
    prt(&prt), bank(&bank), numStudents(numStudents), parentalDelay(parentalDelay){}
