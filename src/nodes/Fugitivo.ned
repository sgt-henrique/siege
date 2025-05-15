package siege.nodes;

import org.car2x.veins.nodes.Car;

module Fugitivo extends Car
{
    parameters:
        @display("i=block/cars;is=s");
        string role = default("fugitivo");
        //string mobility.typename = default("VeinsInetMobility");
        bool hasINET = default(true);
        int numRadios = default(1);
}