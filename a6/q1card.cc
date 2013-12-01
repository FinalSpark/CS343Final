#include "q1classes.h"

    WATCard::WATCard( const WATCard & rhs ){

    }            // prevent copying

    WATCard &WATCard::operator=( const WATCard & rhs ){
        this->balance = rhs.balance;
        return *this;
    }

    WATCard::WATCard(){
        balance = 0;
    }

    void WATCard::deposit( unsigned int amount ){
        balance+=amount;
    }
    void WATCard::withdraw( unsigned int amount ){
        balance-=amount;    // no protection against overdraft
    }
    unsigned int WATCard::getBalance(){
        return balance;
    }
