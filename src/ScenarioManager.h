//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#pragma once

#include "veins/modules/mobility/traci/TraCIScenarioManagerLaunchd.h"
#include "veins/modules/mobility/traci/TraCIMobility.h"

#ifndef SCENARIOMANAGER_H_
#define SCENARIOMANAGER_H_

namespace siege {

class VEINS_API ScenarioManager : public veins::TraCIScenarioManagerLaunchd {

public:
    virtual void initialize(int stage) override;

protected:
    virtual void handleMessage(cMessage* msg) override;
    //virtual void handleSelfMsg(cMessage* msg) override;

    virtual void addCustomVehicle();
    //virtual int numInitStages() const override { return 3; } // ou conforme a base

};

} // namespace veins

#endif /* SCENARIOMANAGER_H_ */
