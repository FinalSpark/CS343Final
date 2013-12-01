    BottlingPlant::BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                 unsigned int timeBetweenShipments ):
        prt(&prt), nameServer(&nameServer), numVendingMachines(numVendingMachines),
        maxShippedPerFlavour(maxShippedPerFlavour), maxStockPerFlavour(maxStockPerFlavour),
        timeBetweenShipments(timeBetweenShipments){

        }

    bool BottlingPlant::getShipment( unsigned int cargo[] ){
        
    }

    void BottlingPlant::main(){
        Truck truck = new Truck(prt, nameServer, *this, numVendingMachines, maxStockPerFlavour);
        while (true) {
            _Accept(getShipment) {

            }
        }
    }
