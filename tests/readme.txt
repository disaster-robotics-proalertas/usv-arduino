
The 'e32' and the 'radiohead' examples were tested with two Arduino nano.

The following parameters from https://github.com/adafruit/RadioHead
where modified to reduce RAM footprint
// changes to reduce ram usage:
//RH_E32_h
//#define RH_E32_MAX_PAYLOAD_LEN 30
//RHDatagram_h
//#define RH_MAX_MESSAGE_LEN 30
//RHRouter_h
//#define RH_DEFAULT_MAX_HOPS 10
//#define RH_ROUTING_TABLE_SIZE 5


pinout

e32       nano
  vdd --- 3.3v
  gnd --- gnd
  tx  --- 7
  rx  --- 6
  m0  --- 4
  m1  --- 5
  aux --- 8
  
  
  
