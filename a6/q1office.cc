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
            //cout << "office create: " << sid << " size:" << jobs.size()  << endl;
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
        while (jobs.size() < 1)
        {
                //cout << "here2" << endl;
          condition.wait();
                //cout << "here3" << endl;
        }
        //return NULL;
        return jobs.front();
    }

    void WATCardOffice::main(){
      prt->print(Printer::WATCardOffice, 'S');
      while (true)
      {
        _Accept (requestWork) {
            jobs.pop();
        } or _Accept (create, transfer) {

        } or _Accept (~WATCardOffice) {
            cout << "reach here1 numcourier: " << numCouriers << endl;
            int dummy = -1;

            for (unsigned int i = 0; i < numCouriers; i++) {
              Job* job = new Job(dummy, 0, NULL);
              jobs.push(job);
              _Accept (requestWork){
                jobs.pop();
              }
            }
            prt->print(Printer::WATCardOffice, 'F');
            break;
        }
      }
    }

    WATCardOffice::~WATCardOffice(){

            //cout << "reach here2 job count" << jobs.size() << endl;
        while (!condition.empty())
        {
          condition.signal();
        }
        for (unsigned int i = 0; i < numCouriers; i++) {
            delete couriers[i];
        }
            //cout << "reach here3" << endl;
        delete[] couriers;
    }



    WATCardOffice::Courier::Courier(Printer *prt, Bank *bank, unsigned int id, WATCardOffice *office):
        prt(prt), bank(bank), id(id), office(office){

        }


    WATCardOffice::Courier::~Courier()
    {

    }
    
    void WATCardOffice::Courier::main(){
        std::vector<struct WATCardOffice::Job *> doneJobs;
        int count = 0;
        prt->print(Printer::Courier, id, 'S');
        while (true){
                //cout << "before request" << endl;
                struct Job* job = office->requestWork();
                //cout << "after request id: " << job->sid << endl;
                if (job->sid == -1)
                {
                  _Accept(~Courier) {
                      //cout << "reach here4" << endl;
                      for (int i = 0; i < count; i++) {
                        delete doneJobs[i];
                      }
                      //cout << "reach here5" << endl;
                  }
                prt->print(Printer::Courier, id, 'F');

                  break; 
                }
                bool lostEh = ran(0, 5) == 0;
                if (lostEh) {
                    //delete job->card;
                    job->result.exception( new Lost );
                } else {
                        //cout << "courier id: " << job->sid << endl;
                    //cout << "middle of courier main id: " << job->sid << endl;
                    prt->print(Printer::Courier, id, 't', job->sid, job->amount);
                    bank->withdraw(job->sid, job->amount);

                    job->card->deposit(job->amount);
                    job->result.delivery(job->card);
                    prt->print(Printer::Courier, id, 'T', job->sid, job->amount);

                }
                //cout << "end of courier main id: " << job->sid << endl;
                doneJobs.push_back(job);
        }
    }

    WATCardOffice::Job::Job( unsigned int sid, unsigned int amount, WATCard *card):
        sid(sid), amount(amount), card(card){

        }
