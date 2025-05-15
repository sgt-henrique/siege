package siege.simulations.siege_basic;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import org.car2x.veins.modules.mobility.traci.TraCIScenarioManagerLaunchd;
import org.car2x.veins.modules.nic.Nic80211p;
import org.car2x.veins.modules.mac.ieee80211p.Mac1609_4;
import org.car2x.veins.modules.phy.PhyLayer80211p;
import org.car2x.veins.modules.mobility.traci.TraCIMobility;

// Importa os nós personalizados
import siege.nodes.Despachante;
import siege.nodes.Fugitivo;
import siege.nodes.Viatura;
import siege.nodes.Radar;
import org.car2x.veins.nodes.Scenario;

network Grid3000Scenario extends Scenario
{
    parameters:
        int numViaturas = default(3);
        int numRadares = default(2);
        @display("bgb=1392,844");
    submodules:
        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=95.975006,198.93001");
        }
        nic: Nic80211p {
            parameters:
                @display("p=80.270004,446.72003");
                *.mac.typename = "Mac1609_4";
                *.phy.typename = "PhyLayer80211p";
//                *.mac.arbitration = true;
//                *.mac.txPower = 20mW;
//                *.phy.sensitivity = -89dBm;
        }


        despachante: Despachante {
            parameters:
                @display("p=333.295,500.81503");
        }

        fugitivo: Fugitivo {
            parameters:
                @display("p=1036.53,240.81001");
        }

        viatura[numViaturas]: Viatura {
            parameters:
                @display("p=949.28,596.79004,row,100");
        }

        radar[numRadares]: Radar {
            parameters:
                @display("p=692.765,303.63,row,100");
        }
}
