#include <uC++.h>
#include "q1classes.h"
#include "MPRNG.h"
#include <vector>

    WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers ):
        prt(&prt), bank(&bank), numCouriers(numCouriers){
        couriers = new Courier*[numCouriers];
        for (int i = 0; i < numCouriers; i++) {
            couriers[i] = new Courier(&prt, &bank, *this);
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
    Job *WATCardOffice::requestWork(){
        return jobs.front();;
    }

    void WATCardOffice::main(){
        _Accept (~WATCardOffice) {

        } or _Accept (transfer, create) {

        } or _When (jobs.size() > 0 ) _Accept (requestWork) {
            jobs.pop();
        }
    }

    WATCardOffice::~WATCardOffice(){
        for (int i = 0; i < numCouriers; i++) {
            delete couriers[i];
        }
        delete[] couriers;
    }



    Courier(Printer &prt, Bank *bank, WATCardOffice *office):
        prt(&prt), bank(&bank), office(&office){

        }
    
    void Courier::main(){
        std::vector<Job *> doneJobs;
        int count = 0;

        while (true){
            _Accept(~Courier) {
                for (int i = 0; i < count; i++) {
                    delete vector[i];
                }
                break;
            } _Else{
                Job* job = office->requestWork();
                bool lostEh = ran(0, 5) == 0;
                if (lostEh) {
                    job->result.exception( new Lost );
                } else {
                    bank->withdraw(job->sid, job->amount);
                    job->result.deliver(job->card->deposit(job->amount));

                }
                doneJobs.push_back(job);
            }
        }
    }

    Job::Job( unsigned int sid, unsigned int amount, WATCard *card):
        sid(sid), amount(amount), card(card){

        }