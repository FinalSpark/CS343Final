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
        return job->result;
    }
    WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ){
        Job* job = new Job(sid, amount, card);
        jobs.push(job);
        return job->result;
    }
struct WATCardOffice::Job *WATCardOffice::requestWork(){
        return jobs.front();
    }

    void WATCardOffice::main(){
        _Accept (~WATCardOffice) {

        } or _Accept (transfer, create) {

        } or _When (jobs.size() > 0 ) _Accept (requestWork) {
            jobs.pop();
        }
    }

    WATCardOffice::~WATCardOffice(){
        for (unsigned int i = 0; i < numCouriers; i++) {
            delete couriers[i];
        }
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
            _Accept(~Courier) {
                for (int i = 0; i < count; i++) {
                    delete doneJobs[i];
                }
                prt->print(Printer::Courier, id, 'F');
                break;
            } _Else{
                struct Job* job = office->requestWork();
                bool lostEh = ran(0, 5) == 0;
                if (lostEh) {
                    job->result.exception( new Lost );
                } else {
                    prt->print(Printer::Courier, id, 't', job->sid, job->amount);
                    bank->withdraw(job->sid, job->amount);
                    job->card->deposit(job->amount);
                    job->result.delivery(job->card);
                    prt->print(Printer::Courier, id, 'T', job->sid, job->amount);

                }
                doneJobs.push_back(job);
            }
        }
    }

    WATCardOffice::Job::Job( unsigned int sid, unsigned int amount, WATCard *card):
        sid(sid), amount(amount), card(card){

        }
