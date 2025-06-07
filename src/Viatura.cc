#include "Viatura.h"
#include "TraCIDemo11pMessage_m.h"

using namespace veins;

Define_Module(veins::Viatura);

void Viatura::initialize(int stage)
{
    BaseApplLayer::initialize(stage);

    if (stage == 0) {
        EV << "Viatura inicializado." << endl;

        annotations = AnnotationManagerAccess().getIfExists();
        ASSERT(annotations);

        mac = FindModule<DemoBaseApplLayerToMac1609_4Interface*>::findSubModule(getParentModule());
        ASSERT(mac);

        findHost()->getDisplayString().setTagArg("i", 1, "red");

        // Leitura dos parâmetros de comunicação
        headerLength = par("headerLength");
        sendBeacons = par("sendBeacons").boolValue();
        beaconLengthBits = par("beaconLengthBits");
        beaconUserPriority = par("beaconUserPriority");
        beaconInterval = par("beaconInterval");

        dataLengthBits = par("dataLengthBits");
        dataOnSch = par("dataOnSch").boolValue();
        dataUserPriority = par("dataUserPriority");

        wsaInterval = par("wsaInterval").doubleValue();
        currentOfferedServiceId = -1;

        isParked = false;  // Pode ser mantido para consistência, mas sem TraCI

        // Inicializa eventos
        sendBeaconEvt = new cMessage("beacon evt", SEND_BEACON_EVT);
        sendWSAEvt = new cMessage("wsa evt", SEND_WSA_EVT);

        // Estatísticas
        generatedBSMs = 0;
        generatedWSAs = 0;
        generatedWSMs = 0;
        receivedBSMs = 0;
        receivedWSAs = 0;
        receivedWSMs = 0;
    }
    else if (stage == 1) {
        myId = mac->getMACAddress();

        if (dataOnSch == true && !mac->isChannelSwitchingActive()) {
            dataOnSch = false;
            EV_ERROR << "Viatura quer enviar na SCH, mas MAC não suporta. Enviando tudo na CCH." << std::endl;
        }

        simtime_t firstBeacon = simTime();

        if (par("avoidBeaconSynchronization").boolValue() == true) {
            simtime_t randomOffset = dblrand() * beaconInterval;
            firstBeacon = simTime() + randomOffset;

            if (mac->isChannelSwitchingActive() == true) {
                if (beaconInterval.raw() % (mac->getSwitchingInterval().raw() * 2)) {
                    EV_ERROR << "O intervalo de beacon (" << beaconInterval << ") não é múltiplo do intervalo de sincronização (" << 2 * mac->getSwitchingInterval() << "). Pode gerar beacons na SCH!" << std::endl;
                }
                firstBeacon = computeAsynchronousSendingTime(beaconInterval, ChannelType::control);
            }
        }

        if (sendBeacons) {
            scheduleAt(firstBeacon, sendBeaconEvt);
        }
    }
}

