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
        return job->result;
    }
    WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ){
        Job* job = new Job(sid, amount, card);
        jobs.push(job);

        prt->print(Printer::WATCardOffice, 'T', sid, amount);
        return job->result;
    }
struct WATCardOffice::Job *WATCardOffice::requestWork(){
        return jobs.front();
    }

    void WATCardOffice::main(){
      prt->print(Printer::WATCardOffice, 'S');
      while (true)
      {
        _Accept (~WATCardOffice) {
            cout << "reach here1" << endl;
            int dummy = -1;

            for (unsigned int i = 0; i < numCouriers; i++) {
               create(dummy, 0);
              /*Job* job = new Job(dummy, 0, NULL);
              jobs.push(job);*/
              /*_Accept (requestWork){
                jobs.pop();
              }*/
            }
            prt->print(Printer::WATCardOffice, 'F');
            break;
        } or _Accept (create, transfer) {

        } or _When (jobs.size() > 0 ) _Accept (requestWork) {
            jobs.pop();
        }
      }
    }

    WATCardOffice::~WATCardOffice(){

            cout << "reach here2 job count" << jobs.size() << endl;
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

    }
    
    void WATCardOffice::Courier::main(){
        std::vector<struct WATCardOffice::Job *> doneJobs;
        int count = 0;
        prt->print(Printer::Courier, id, 'S');
        while (true){
                struct Job* job = office->requestWork();
                if (job->sid == -1)
                {
                  _Accept(~Courier) {
                      cout << "reach here4" << endl;
                      for (int i = 0; i < count; i++) {
                        delete doneJobs[i];
                      }
                  }
                prt->print(Printer::Courier, id, 'F');

                  continue;
                }
                bool lostEh = ran(0, 5) == 0;
                if (lostEh) {
                    job->result.exception( new Lost );
                } else {
                        //cout << "courier id: " << job->sid << endl;
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
