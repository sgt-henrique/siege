//
// Copyright (C) 2018 Christoph Sommer <sommer@ccs-labs.org>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// SPDX-License-Identifier: GPL-2.0-or-later
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#include "veins_inet/Basic.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/TagBase_m.h"
#include "inet/common/TimeTag_m.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/networklayer/common/L3AddressTag_m.h"
#include "inet/transportlayer/contract/udp/UdpControlInfo_m.h"
#include "veins_inet/VeinsInetSampleMessage_m.h"

using namespace inet;
using namespace veins;
using namespace std;

Define_Module(Basic);

Basic::Basic()
{
}

bool Basic::startApplication()
{
    // fazer surgir dinamicamente 2 viaturas no instante 5
    // fazer surgir dinamicamente um veículo no instante 5s (armazenar sua localização e o sentido na via)

    // 1. Adiciona o veículo ao SUMO
    TraCIScenarioManager* manager = FindModule<TraCIScenarioManager*>::findGlobalModule();

    EV << "Vai tentar adicionar o fugitivo depois do tempo 5";

    if (simTime() > 5) {
        EV << "está tentando";
        string id = "fugitivo";
        double departTime = simTime().dbl();

        //lista de edges
        vector<string> edgesVec = {
                "0/4to0/5",
                "0/5to1/5",
                "1/5to1/4",
                "1/4to1/3",
                "1/3to1/2",
                "1/2to2/2"
        };

        // Passo 2: converter para std::list
        list<string> edgesList(edgesVec.begin(), edgesVec.end());

        // Passo 3: adicionar rota
        string routeId = "rotaDinamica";
        manager->getCommandInterface()->addRoute(routeId, edgesList);

        //manager->getCommandInterface()->vehicle().add
        manager->getCommandInterface()->addVehicle(
            id,           // vehicleId
            "car",          // vehicleTypeId (deve estar no .rou.xml)
            routeId,      // routeId (deve estar no .rou.xml)
            departTime,       // tempo atual ou no futuro
            0.0,           // posição na via (em metros)
            10.0,           // velocidade inicial
            0               // faixa (0 = primeira)
            );
        if (strcmp(getParentModule()->getFullName(), "fugitivo") == 0) {
            getParentModule()->getDisplayString().setTagArg("i", 1, "red");
        }
    }


    // 2. Define tipo de veículo, posição, etc. (opcional)
    //manager->getCommandInterface()->vehicle.setType(id, "car");
    //manager->getCommandInterface()->vehicle.setSpeed(id, 10.0);
    //manager->getCommandInterface()->vehicle.moveToXY(id, "edge1", 0, 10.0, 10.0, 0.0);

    // O módulo VeinsInetCar (ou outro que você definiu em `*.manager.moduleType`)
    // será criado automaticamente pelo TraCIScenarioManager

    // aguardar o tempo da solicitação da vítima
    // despachante comunica para todos os nós da rede a última localiação e o sentido na via
    // aguardar o tempo do CPU processar a notícia e definir uma estratégia
    // CPU comunica para todos os nós da rede a última estratégia
    // aguardar o tempo das viaturas processarem a notícia e adotar a estratégia


    // host[0] should stop at t=20s
    /*if (getParentModule()->getIndex() == 1) {
        auto callback = [this]() {
            getParentModule()->getDisplayString().setTagArg("i", 1, "red");

            traciVehicle->setSpeed(0);

            auto payload = makeShared<VeinsInetSampleMessage>();
            payload->setChunkLength(B(100));
            payload->setRoadId(traciVehicle->getRoadId().c_str());
            timestampPayload(payload);

            auto packet = createPacket("accident");
            packet->insertAtBack(payload);
            sendPacket(std::move(packet));

            // host should continue after 30s
            auto callback = [this]() {
                traciVehicle->setSpeed(-1);
            };
            timerManager.create(veins::TimerSpecification(callback).oneshotIn(SimTime(30, SIMTIME_S)));
        };
        timerManager.create(veins::TimerSpecification(callback).oneshotAt(SimTime(20, SIMTIME_S)));
    }*/

    return true;
}

bool Basic::stopApplication()
{
    return true;
}

Basic::~Basic()
{
}

void Basic::processPacket(std::shared_ptr<inet::Packet> pk)
{
    auto payload = pk->peekAtFront<VeinsInetSampleMessage>();

    EV_INFO << "Received packet: " << payload << endl;

    getParentModule()->getDisplayString().setTagArg("i", 1, "green");

    traciVehicle->changeRoute(payload->getRoadId(), 999.9);

    if (haveForwarded) return;

    auto packet = createPacket("relay");
    packet->insertAtBack(payload);
    sendPacket(std::move(packet));

    haveForwarded = true;
}
