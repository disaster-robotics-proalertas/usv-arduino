
The PWM lib comes from https://github.com/xkam1x/Arduino-PWM-Reader.
Some modifications were made, but probably works with default ones too.

The RadioHead lib from https://github.com/adafruit/RadioHead
was modified to reduce RAM footprint
// changes to reduce ram usage:
//RH_E32_h
//#define RH_E32_MAX_PAYLOAD_LEN 50
//RHDatagram_h
//#define RH_MAX_MESSAGE_LEN 50
//RHRouter_h
//#define RH_DEFAULT_MAX_HOPS 10
//#define RH_ROUTING_TABLE_SIZE 5

SetNodeId is a utility (from https://github.com/nootropicdesign/lora-mesh) to set the nodes ID via EEPROM.
It will be usefull in the future.
