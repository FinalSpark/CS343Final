#include <uC++.h>
#include "q1classes.h"
#include "MPRNG.h"
#include <vector>

   /**
    * WATCardOffice::(constructor)
    * initialize {numCouriers} couriers
    */
  WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers ):
        prt(&prt), bank(&bank), numCouriers(numCouriers){
        couriers = new Courier*[numCouriers];
        for (unsigned int i = 0; i < numCouriers; i++) {
            couriers[i] = new Courier(&prt, &bank, i, this);
        }
        terminated = false;
    }

   /**
    * WATCardOffice::create
    * create job of creating new card for student {sid} with {amount} dollars.
    * Job is queued for couriers in queue<job> jobs.
    * After job is queued, signal condition
    */
    WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ){
        Job* job = new Job(sid, amount, new WATCard());
        jobs.push(job);
        prt->print(Printer::WATCardOffice, 'C', sid, amount);
        condition.signal();
        return job->result;
    }

   /**
    * WATCardOffice::transfer
    * create job of transfer {amount} dollars from student {sid}'s account to watcard.
    * Job is queued for couriers in queue<job> jobs.
    * After job is queued, signal condition
    */
    WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ){
        Job* job = new Job(sid, amount, card);
        jobs.push(job);

        prt->print(Printer::WATCardOffice, 'T', sid, amount);
        condition.signal();
        return job->result;
    }

   /**
    * WATCardOffice::requestWork
    * If office is closed (terminated == true), do not wait, pickup the dummy (terminate) job.
    * If there are jobs in the queue, do not wait, picku put the job.
    * Otherwise, wait till there are jobs 
    */
    struct WATCardOffice::Job *WATCardOffice::requestWork(){
        while (jobs.size() < 1 && !terminated)
        {
          condition.wait();
        }
        if (terminated) return NULL;
        struct WATCardOffice::Job * temp = jobs.front();
        //return NULL;
        jobs.pop();
        prt->print(Printer::WATCardOffice, 'W');
        return temp;
    }

   /**
    * WATCardOffice::main
    * Accept calls
    * After destructor is called, remove all queued jobs (no need to complete pending tasks since students are all finished at this point)
    * Push dummy jobs to the queue to signal couriers to terminate.
    * Set the office's status to terminated and wake up all tasks that are waiting on condition.
    * Couriers will pickup the dummy task after being waken up here.
    */
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
            terminated = true;
            int courierCount = 0;
            while (!condition.empty())
            {
              condition.signalBlock();
              courierCount++;
            }
            courierCount = numCouriers-courierCount;
            while (courierCount > 0) {
                _Accept(requestWork){
                    courierCount--;
                }
            }
            for (unsigned int i = 0; i < numCouriers; i++) {
            delete couriers[i];
            }
            delete[] couriers;
            prt->print(Printer::WATCardOffice, 'F');
            break;
        }
      }
    }

    WATCardOffice::~WATCardOffice(){

    }


    WATCardOffice::Courier::Courier(Printer *prt, Bank *bank, unsigned int id, WATCardOffice *office):
        prt(prt), bank(bank), id(id), office(office){

        }


    WATCardOffice::Courier::~Courier()
    {
        prt->print(Printer::Courier, id, 'F');
    }
    

   /**
    * Courier::main
    * Loop for jobs.
    * After getting job, try withdraw money, simulate lost.
    * If lost, add exception to future, otherwise, deposit money and deliver the card
    * In either cases, the job is recorded for memory deallocation
    */
    void WATCardOffice::Courier::main(){
        std::vector<struct WATCardOffice::Job *> doneJobs;
        prt->print(Printer::Courier, id, 'S');
        while (true){
                struct Job* job = office->requestWork();
                if ( job == NULL || job->sid == -1)
                {
                    for (int i = 0; i < doneJobs.size(); i++) {
                        delete doneJobs[i];
                    }
                  break; 
                }
                prt->print(Printer::Courier, id, 't', job->sid, job->amount);
                bank->withdraw(job->sid, job->amount);
                bool lostEh = ran(0, 5) == 0;
                if (lostEh) {
                    //delete job->card;
                    job->result.exception( new Lost );
                } else {
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
