#pragma once

#include "veins/modules/application/ieee80211p/DemoBaseApplLayer.h"

#ifndef VIATURA_H_
#define VIATURA_H_
namespace veins {


class VEINS_API Viatura : public DemoBaseApplLayer {
public:
    void initialize(int stage) override;

protected:
    simtime_t lastDroveAt;
    bool sentMessage;
    int currentSubscribedServiceId;

/*protected:
    void onWSM(BaseFrame1609_4* wsm) override;
    void onWSA(DemoServiceAdvertisment* wsa) override;

    void handleSelfMsg(cMessage* msg) override;
    void handlePositionUpdate(cObject* obj) override;*/
};
}

#endif /* FUGITIVO_H_ */
