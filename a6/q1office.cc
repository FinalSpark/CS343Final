#include <uC++.h>
#include "q1classes.h"
#include "MPRNG.h"
#include <vector>

  WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers ):
        prt(&prt), bank(&bank), numCouriers(numCouriers){
        couriers = new Courier*[numCouriers];
        for (unsigned int i = 0; i < numCouriers; i++) {
            couriers[i] = new Courier(&prt, &bank, i, this);
        }
    }

    WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ){
        Job* job = new Job(sid, amount, new WATCard());
        jobs.push(job);
        prt->print(Printer::WATCardOffice, 'C', sid, amount);
        condition.signal();
        return job->result;
    }
    WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ){
        Job* job = new Job(sid, amount, card);
        jobs.push(job);

        prt->print(Printer::WATCardOffice, 'T', sid, amount);
        condition.signal();
        return job->result;
    }
struct WATCardOffice::Job *WATCardOffice::requestWork(){
                //cout << "here1" << endl;
        cout << jobs.size() << endl;
        while (jobs.size() < 1)
        {
          condition.wait();
        }
        struct WATCardOffice::Job * temp = jobs.front();
        //return NULL;
        jobs.pop();
        prt->print(Printer::WATCardOffice, 'W');
        return temp;
    }

    void WATCardOffice::main(){
      prt->print(Printer::WATCardOffice, 'S');
      while (true)
      {
        _Accept (requestWork) {
        } or _Accept (create, transfer) {

        } or _Accept (~WATCardOffice) {
            int dummy = -1;
            while (jobs.size() > 0) {
                delete jobs.front();
                jobs.pop();
            }
            for (unsigned int i = 0; i < numCouriers; i++) {
              Job* job = new Job(dummy, 0, NULL);
              jobs.push(job);
            }
            for (unsigned int i = 0; i < numCouriers; i++) {
              cout<<"dummy size:"<<jobs.size()<<endl;
              _Accept(requestWork){

              }
            }
            prt->print(Printer::WATCardOffice, 'F');
            break;
        }
      }
    }

    WATCardOffice::~WATCardOffice(){

            cout << "reach here2 job count" << jobs.size() << endl;
        while (!condition.empty())
        {
          condition.signalBlock();
        }
        for (unsigned int i = 0; i < numCouriers; i++) {
            delete couriers[i];
        }
            cout << "reach here3" << endl;
        delete[] couriers;
    }



    WATCardOffice::Courier::Courier(Printer *prt, Bank *bank, unsigned int id, WATCardOffice *office):
        prt(prt), bank(bank), id(id), office(office){

        }


    WATCardOffice::Courier::~Courier()
    {
        prt->print(Printer::Courier, id, 'F');
    }
    
    void WATCardOffice::Courier::main(){
        std::vector<struct WATCardOffice::Job *> doneJobs;
        int count = 0;
        prt->print(Printer::Courier, id, 'S');
        while (true){
                cout<<"requesting job"<<endl;
                struct Job* job = office->requestWork();
                if (job->sid == -1)
                {
                    for (int i = 0; i < count; i++) {
                        delete doneJobs[i];
                    }
                  break; 
                }
                bool lostEh = ran(0, 5) == 0;
                if (lostEh) {
                    //delete job->card;
                    job->result.exception( new Lost );
                } else {
                    cout << "courier id: " << job->sid << endl;
                    prt->print(Printer::Courier, id, 't', job->sid, job->amount);
                    bank->withdraw(job->sid, job->amount);

                    job->card->deposit(job->amount);
                    job->result.delivery(job->card);
                    prt->print(Printer::Courier, id, 'T', job->sid, job->amount);

                }
                doneJobs.push_back(job);
        }
    }

    WATCardOffice::Job::Job( unsigned int sid, unsigned int amount, WATCard *card):
        sid(sid), amount(amount), card(card){

        }
