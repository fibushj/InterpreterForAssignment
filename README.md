# FlightGear Simulator
An interpreter for a fictive language used to communicate bidirectionally with the FlightGear simulator.
The interpreter implementation involved the Shunting Yard algorithm for evaluating mathematical expressions.
Example of a possible input to the interpreter:
```
openDataServer 5400 10
connect 127.0.0.1 5402
var breaks = bind "/controls/flight/speedbrake"
var throttle = bind "/controls/engines/engine/throttle"
var heading = bind "/instrumentation/heading-indicator/offset-deg"
var airspeed = bind "/instrumentation/airspeed-indicator/indicated-speed-kt"
var roll = bind "/instrumentation/attitude-indicator/indicated-roll-deg"
var pitch = bind "/instrumentation/attitude-indicator/internal-pitch-deg"
var rudder = bind "/controls/flight/rudder"
var aileron = bind "/controls/flight/aileron"
var elevator = bind "/controls/flight/elevator"
var alt = bind "/instrumentation/altimeter/indicated-altitude-ft"
breaks = 0
throttle = 1
var h0 = heading
while alt < 1000 {
    rudder = (h0 – heading)/20
    aileron = - roll / 70
    elevator = pitch / 50
    print alt
    sleep 250
}
print "done"
```
