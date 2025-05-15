package siege.nodes;

import org.car2x.veins.nodes.RSU;


module Radar extends RSU
{
    parameters:
        @display("i=block/tower;is=s");
        string role = default("radar");
        bool hasINET = default(true);
        int numRadios = default(2); // VANET + LTE
        //string mobility.typename = default("StationaryMobility");
}