#include "ScenarioManager.h"

Define_Module(siege::ScenarioManager);

using namespace std;
using namespace veins;

namespace siege {

void ScenarioManager::initialize(int stage) {

    TraCIScenarioManagerLaunchd::initialize(stage);
    EV << "Meu ScenarioManager (siege) foi inicializado!" << endl;

    if (stage == 0) {
        // Agendar evento para criar veículo após 5 segundos
        cMessage* addVehicleMsg = new cMessage("addVehicle");
        scheduleAt(simTime() + 5, addVehicleMsg);
    }
}

void ScenarioManager::handleMessage(cMessage* msg) {
    EV << "ScenarioManager: mensagem recebida: " << msg->getName() << endl;

    if (strcmp(msg->getName(), "addVehicle") == 0) {
        EV << "Entrou no if: ";
        addCustomVehicle();
        //delete msg;
        return;
    }

    if (msg->isSelfMessage()) {
        handleSelfMsg(msg);
        return;
    }
    throw cRuntimeError("erro no handleMessage de ScenarioManager)");
}

void ScenarioManager::addCustomVehicle() {
    EV << "Adicionando veículo vermelho na simulação." << endl;

    //Atributos do veículo
    std::string vehicleId = "fugitivo";
    std::string vehicleType = "car";
    std::string routeId = "route0";
    double speed = 13.89;  // 50 km/h
    Coord headingVector(1, 0, 0);  // direção para o leste
    Heading heading = Heading::fromCoord(headingVector);

    VehicleSignalSet signals;  // pode ser vazio se não usar sinais

    double length = 4.5;  // exemplo: 4.5 metros
    double height = 1.5;  // exemplo: 1.5 metros
    double width = 1.8;   // exemplo: 1.8 metros

    // 1) Adiciona veículo no SUMO via TraCI (departTime=0 para sair imediatamente)
    getCommandInterface()->addVehicle(vehicleId, vehicleType, routeId, 0.0);

    // 2) Posição inicial do veículo (ajuste conforme sua necessidade)
    Coord initialPos(0, 0);

    // 3) Cria o módulo OMNeT++ associado ao veículo
    // moduleType, moduleName e displayString precisam existir no seu .ini ou código
    std::string moduleType = "org.car2x.veins.nodes.Car";  // Exemplo, ajuste conforme seu módulo de veículo
    std::string moduleName = vehicleId;
    std::string displayString = "";

    EV << "Veículo " << vehicleId << " adicionado2" << endl;
    addModule(vehicleId, moduleType, moduleName, displayString, initialPos, routeId, speed, heading, signals, length, height, width);

    EV << "Veículo " << vehicleId << " adicionado e colorido de vermelho." << endl;
}

} // namespace veins


