#ifndef __CLASSES_H__
#define __CLASSES_H__ 1

#include <uC++.h>
#include <vector>
#include <uFuture.h>
#include <map>
#include <queue>

using std::map;
using std::vector;

_Task NameServer;
_Task BottlingPlant;

_Monitor Printer {
  public:
    enum Kind { Parent, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
    Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
    ~Printer();
    void print( Kind kind, char state );
    void print( Kind kind, char state, int value1 );
    void print( Kind kind, char state, int value1, int value2 );
    void print( Kind kind, unsigned int lid, char state );
    void print( Kind kind, unsigned int lid, char state, int value1 );
    void print( Kind kind, unsigned int lid, char state, int value1, int value2 );
  private:
    unsigned int numStudents;
    unsigned int numVendingMachines;
    unsigned int numCouriers;
    map<Kind, char> sGeneral;
    map<int, char> sStudent;
    map<int, char> sVending;
    map<int, char> sCourier;
    map<Kind, int> generalNumber1;
    map<Kind, int> generalNumber2;
    map<int, int> studentNumber1;
    map<int, int> studentNumber2;
    map<int, int> vendingNumber1;
    map<int, int> vendingNumber2;
    map<int, int> courierNumber1;
    map<int, int> courierNumber2;
    void flush();                                            //called by print to print a line;
    void flushFinish(Kind kind, unsigned int lid);           //called by print to print finish
};

class WATCard {
    unsigned int balance;
    WATCard( const WATCard & );            // prevent copying
    WATCard &operator=( const WATCard & );
  public:
    WATCard();
    typedef Future_ISM<WATCard *> FWATCard; // future watcard pointer
    void deposit( unsigned int amount );
    void withdraw( unsigned int amount );
    unsigned int getBalance();
};

_Task VendingMachine {
    Printer *prt;
    NameServer *nameServer;
    unsigned int id;
    unsigned int sodaCost;
    unsigned int stock[4];
    bool restocking;
    bool buySuccess;

    void main();
  public:
    enum Flavours { COKE, ICETEA, DRPEPPER, SPRITE };                 // flavours of soda (YOU DEFINE)
    enum Status { BUY, STOCK, FUNDS };     // purchase status: successful buy, out of stock, insufficient funds
    VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
                    unsigned int maxStockPerFlavour );
    Status buy( Flavours flavour, WATCard &card );
    unsigned int *inventory();
    void restocked();
    _Nomutex unsigned int cost();
    _Nomutex unsigned int getId();
  private:
    Flavours buyFlavour;
};

_Task NameServer {
    VendingMachine **vmList;
    unsigned int counter;
    unsigned int vmCounter;
    Printer *prt;
    unsigned int numVendingMachines;
    unsigned int numStudents;
    unsigned int* assignment;
    void main();
    unsigned int sid;
    unsigned int vid;
  public:
    NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents );
    void VMregister( VendingMachine *vendingmachine );
    VendingMachine *getMachine( unsigned int id );
    VendingMachine **getMachineList();
    ~NameServer();
};

_Monitor Bank {
    unsigned int numStudents;
    std::vector<unsigned int> studentBalance;
    std::vector<uCondition*> condition;
  public:
    Bank( unsigned int numStudents );
    void deposit( unsigned int id, unsigned int amount );
    void withdraw( unsigned int id, unsigned int amount );
};

_Task WATCardOffice {
    struct Job {                           // marshalled arguments and return future
        unsigned int sid;
        unsigned int amount;
        WATCard *card;
        WATCard::FWATCard result;                   // return future
        Job( unsigned int sid, unsigned int amount, WATCard *card);
    };
    _Task Courier { 
        Printer *prt;
        Bank *bank;
        unsigned int id;
        WATCardOffice *office;
        void main();
      public:
        Courier(Printer *prt, Bank *bank, unsigned int id, WATCardOffice *office);
        ~Courier();
    };                 // communicates with bank

    void main();
    Printer * prt;
    Bank * bank;
    unsigned int numCouriers;
    std::queue<Job*> jobs;
    Courier **couriers;
  public:
    _Event Lost {};                        // uC++ exception type, like "struct"
    WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );
    WATCard::FWATCard create( unsigned int sid, unsigned int amount );
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
    Job *requestWork();
    ~WATCardOffice();
};

_Task Student {
        Printer *prt;
        NameServer *nameServer;
        WATCardOffice *cardOffice;
        unsigned int id;
        unsigned int maxPurchases;
        void main();
    public:
        Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id,
        unsigned int maxPurchases );
};

_Task Parent {
    Printer* prt;
    Bank *bank;
    int numStudents;
    unsigned int parentalDelay;
    void main();
  public:
    Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay );
};


_Task Truck {
    void main();
    unsigned int cargo[4];
    Printer *prt;
    NameServer *nameServer;
    BottlingPlant *plant;
    unsigned int numVendingMachines;
    unsigned int maxStockPerFlavour;
  public:
    Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
           unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
};

_Task BottlingPlant {
    Printer *prt;
    NameServer *nameServer;
    unsigned int numVendingMachines;
    unsigned int maxShippedPerFlavour;
    unsigned int maxStockPerFlavour;
    unsigned int timeBetweenShipments;
    unsigned int soda[4];
    Truck* truck;
    bool closed;
    void main();
  public:
    BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                 unsigned int timeBetweenShipments );
    bool getShipment( unsigned int cargo[] );
    ~BottlingPlant();
};


#endif
