package siege.nodes;

import org.car2x.veins.nodes.RSU;

module Despachante extends RSU
{
    parameters:
        @display("i=block/control;is=s");
        string role = default("despachante");
        bool hasINET = default(true);
        int numRadios = default(1);
        //string mobility.typename = default("StationaryMobility");
}