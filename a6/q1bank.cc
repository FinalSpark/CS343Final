#include <uC++.h>
#include "q1classes.h"
#include "MPRNG.h"


/**
 * Each student account has a condition variable. Call to withdraw is blocked until there is sufficient fund
 */
void Bank::deposit( unsigned int id, unsigned int amount ){
    studentBalance[id]+=amount;
    condition[id]->signal();
}


void Bank::withdraw( unsigned int id, unsigned int amount ){
    while (amount > studentBalance[id]) {
        condition[id]->wait();
    }
    studentBalance[id]-=amount;
}


Bank::Bank( unsigned int numStudents ):numStudents(numStudents){
    for (unsigned int i = 0; i<numStudents; i++) {
        studentBalance.push_back(0);
        condition.push_back(new uCondition());
    }
}

Bank::~Bank(){
  for (int i = 0; i < condition.size(); i++)
  {
    delete condition.at(i);
  }

}
