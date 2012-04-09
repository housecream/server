/**
 * HouseCream arduino software.
 * 
 * @author n0rad
 */
 
#include "etherShield.h"

////////////////////////////////////////////////////////////////////////////////////////
// structs and define that is needed by conf and can not be under configuration
////////////////////////////////////////////////////////////////////////////////////////
typedef struct s_pin {
  char         mode; // NOTUSED (-1), INPUT (0) , OUTPUT(1), PWM (2), ANALOG (3), DIGITAL (4)
  int          startValue; // 0-1 for digital, 0-255 for PWM
  char         *description;
} t_pin;

typedef struct s_pin2 {
  char         direction; // NOTUSED(-1), INPUT(0) , OUTPUT(1), RESERVED(2)
  char         type; // ANALOG(1), DIGITAL(2)

  int          startValue; // 0-1 for digital, 0-255 for PWM
  char         *description;
} t_pin2;

#define NOTUSED -1
#define PWM 2
#define ANALOG 3
#define DIGITAL 4

////////////////////////////////////////////////////////////////////////////////////////
// This is the configuration part
////////////////////////////////////////////////////////////////////////////////////////

// DEBUG on if defined
//#define DEBUG

static uint8_t   mac[6]         = {0x54, 0x55, 0x58, 0x10, 0x00, 0xF4}; 
static uint8_t   ip[4]          = {192, 168, 42, 245};
static uint16_t  port           = 80;
static char      description[]  = "control everything on window1";
static char      name[]  = "window1 controller";
static char      notifyUrl[]  = "";
static char      technicalDescription[]  = "indoor management of the window only, not powered from POE";

static uint16_t p_pinSize = 20;

/**
 * DIGITAL pin as input and value 1 will enable 20k pullup
 */
t_pin	p_pin[] = {
// DIGITAL
/*  0 */ {INPUT, HIGH, "TEST1"},
/*  1 */ {INPUT, LOW, "TEST2"},
/*  2 */ {OUTPUT, HIGH, "TEST3"},
/*  3 */ {PWM, 255, "TEST4"}, // PWM
/*  4 */ {NOTUSED, 0, 0},
/*  5 */ {OUTPUT, HIGH, "TEST5"}, // PWM
/*  6 */ {PWM, 255, "TEST6"}, // PWM
/*  7 */ {OUTPUT, HIGH, "SIMPLE LED"},
/*  8 */ {OUTPUT, HIGH, "TEST7"},
/*  9 */ {OUTPUT, LOW, "TEST9"}, // PWM
/* 10 */ {NOTUSED, 0, 0}, // Used by ethernet  // PWM
/* 11 */ {NOTUSED, 0, 0}, // Used by ethernet  // PWM
/* 12 */ {NOTUSED, 0, 0}, // Used by ethernet
/* 13 */ {NOTUSED, 0, 0}, // Used by ethernet
// ANALOG
/* 14 */ {ANALOG, 0, "temp receptor 1"}, // A0
/* 15 */ {DIGITAL, 0, "temp receptor 2"}, // A1
/* 16 */ {NOTUSED, 0, "temp receptor 3"}, // A2
/* 17 */ {ANALOG, 0, "temp receptor 4"}, // A3
/* 18 */ {DIGITAL, 0, "temp receptor 5"}, // A4
/* 19 */ {ANALOG, 0, "temp receptor 6"}  // A5
};

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
#ifdef DEBUG
  #define DEBUG_PRINT(x)      Serial.print(x)
  #define DEBUG_PRINTDEC(x)   Serial.print(x, DEC)
  #define DEBUG_PRINTLN(x)    Serial.println(x)
  #define DEBUG_WRITE(x, y)   Serial.write(x, y)
  #define DEBUG_HEXDUMP(x, y) hexDump(x, y)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTDEC(x)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_WRITE(x, y)
  #define DEBUG_HEXDUMP(x, y)
#endif

char globalErrorBuffer[100] = {0};
EtherShield es = EtherShield();



#define GET "GET "
#define PUT "PUT "
#define HCC_VERSION "0.1"
#define HARDWARE "Arduino Duemilanove / Nuelectronics enc28j60 Ethernet Shield V1.1"


typedef struct s_resource {
  int id;
  char *method;
  char *query;
  uint16_t (*func)(uint8_t *buf, uint16_t dat_p, uint16_t plen);
} t_resource;


uint16_t rootGet(uint8_t *buf, uint16_t dat_p, uint16_t plen) {
  plen = es.ES_fill_tcp_data_p(buf, 0, PSTR("HTTP/1.0 200 OK\r\nContent-Type: application/json\r\n\r\n"));
  plen = addToBuffer(buf, plen, "{\"software\":\"HouseCream Client\", \"version\":\"");
  plen = addToBuffer(buf, plen, HCC_VERSION);

  plen = addToBuffer(buf, plen, "\",\"hardware\":\"");
  plen = addToBuffer(buf, plen, HARDWARE);
  
  plen = addToBuffer(buf, plen, "\",\"name\":\"");
  plen = addToBuffer(buf, plen, name);
  
  plen = addToBuffer(buf, plen, "\",\"description\":\"");
  plen = addToBuffer(buf, plen, description);

  plen = addToBuffer(buf, plen, "\",\"notifyUrl\":\"");
  plen = addToBuffer(buf, plen, notifyUrl);

  plen = addToBuffer(buf, plen, "\",\"technicalDescription\":\"");
  plen = addToBuffer(buf, plen, technicalDescription);

  plen = addToBuffer(buf, plen, "\",\"ip\":\"");
  plen = addToBuffer(buf, plen, ip[0]);
  plen = addToBuffer(buf, plen, ".");
  plen = addToBuffer(buf, plen, ip[1]);
  plen = addToBuffer(buf, plen, ".");
  plen = addToBuffer(buf, plen, ip[2]);
  plen = addToBuffer(buf, plen, ".");
  plen = addToBuffer(buf, plen, ip[3]);

  plen = addToBuffer(buf, plen, "\",\"port\":\"");
  plen = addToBuffer(buf, plen, port);

  plen = addToBuffer(buf, plen, "\",\"numberOfPin\":\"");
  plen = addToBuffer(buf, plen, p_pinSize);
  
//  plen = addToBuffer(buf, plen, "\",\"mac\":\"");
//  plen = addToBufferHex(buf, plen, mac[0]);
//  plen = addToBufferHex(buf, plen, mac[1]);
//  plen = addToBufferHex(buf, plen, mac[2]);
//  plen = addToBufferHex(buf, plen, mac[3]);
//  plen = addToBufferHex(buf, plen, mac[4]);
//  plen = addToBufferHex(buf, plen, mac[5]);

  plen = addToBuffer(buf, plen, "\"}");
//    private String mac;
   
  return plen;
}

#define BUFFER_SIZE2 100
static uint8_t buf2[BUFFER_SIZE2 + 1];



uint16_t pinGetDescription(uint8_t *buf, uint16_t pinId) {
  t_pin current = p_pin[pinId];
  uint16_t plen = es.ES_fill_tcp_data_p(buf, 0, PSTR("HTTP/1.0 200 OK\r\nContent-Type: application/json\r\n\r\n"));

  plen = addToBuffer(buf, plen, "{\"technicalDescription\":\"");
  plen = addToBuffer(buf, plen, current.technicalDescription);

  plen = addToBuffer(buf, plen, "\",\"direction\":\"");
  plen = addToBuffer(buf, plen, current.direction);

  plen = addToBuffer(buf, plen, "\",\"type\":\"");
  plen = addToBuffer(buf, plen, current.type);

  plen = addToBuffer(buf, plen, "\",\"pullUp\":\"");
  plen = addToBuffer(buf, plen, current.pullUp);

  plen = addToBuffer(buf, plen, "\",\"valueMin\":\"");
  plen = addToBuffer(buf, plen, current.valueMin);

  plen = addToBuffer(buf, plen, "\",\"valueMax\":\"");
  plen = addToBuffer(buf, plen, current.valueMax);

  plen = addToBuffer(buf, plen, "\",\"valueStep\":\"");
  plen = addToBuffer(buf, plen, current.valueStep);

  plen = addToBuffer(buf, plen, "\"}");
  return plen;
}


uint16_t pinGet(uint8_t *buf, uint16_t dat_p, uint16_t plen) {
  int pinId;
  char request[20] = {0};
  
  //sscanf will always return 0 or 2 because it read HTTP/1.1 when there is nothing after the pinId
  int found = sscanf((char *) &buf[dat_p + 5], "%d%s", &pinId, request);
  if (found != 0) {
    if (pinId < 0 || pinId > p_pinSize - 1) {
      plen = es.ES_fill_tcp_data_p(buf, 0, PSTR("HTTP/1.0 400 OK\r\nContent-Type: application/json\r\n\r\n"));
      plen = addToBuffer(buf, plen, "{\"message\":\"PinId overflow\"}");
      return plen;
    }
    if ((request[0] == '/' && !request[1]) || strncmp("HTTP/", request, 5) == 0) {
      plen = es.ES_fill_tcp_data_p(buf, 0, PSTR("HTTP/1.0 200 OK\r\nContent-Type: application/json\r\n\r\n"));
      plen = addToBuffer(buf, plen, "{root}");
      return plen;
    } else if (strncmp("/value", request, 5) == 0) {
      plen = es.ES_fill_tcp_data_p(buf, 0, PSTR("HTTP/1.0 200 OK\r\nContent-Type: application/json\r\n\r\n"));
      plen = addToBuffer(buf, plen, getValue(pinId));
      return plen;
    } else if (strncmp("/info", request, 4) == 0) {
      plen = es.ES_fill_tcp_data_p(buf, 0, PSTR("HTTP/1.0 200 OK\r\nContent-Type: application/json\r\n\r\n"));
      plen = addToBuffer(buf, plen, "{info}");
      return plen;
    } else {
      plen = es.ES_fill_tcp_data_p(buf, 0, PSTR("HTTP/1.0 404 OK\r\nContent-Type: application/json\r\n\r\n"));
      plen = addToBuffer(buf, plen, "{\"message\":\"404 No resource on pin for this method & url\"}");
      return plen;      
    }
  } else {
    plen = es.ES_fill_tcp_data_p(buf, 0, PSTR("HTTP/1.0 400 OK\r\nContent-Type: application/json\r\n\r\n"));
    plen = addToBuffer(buf, plen, "{\"message\":\"Cannot read pin number in the request\"}");
    return plen;
  }
  return plen;
}


t_resource	p_resource[] = {
  {1, GET, "/ ", rootGet},
  {2, GET, "/pin/", pinGet},
//  {PUT, "/ ",  },

  {0, 0, 0, 0}
};


/////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  #ifdef DEBUG
    Serial.begin(9600);
    delay(2000);
  #endif

  // init ethernet shield
  init28j60EthernetShield();
  
  if (checkDef()) {
      DEBUG_PRINTLN(globalErrorBuffer);
      return;
  }
  // init pins
  initPins();
}

void loop() {
  // manage web server
  netServerManager();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

#define BUFFER_SIZE 1000
static uint8_t buf[BUFFER_SIZE + 1];



/**
 *
 */
void netServerManager() {
  uint16_t plen, dat_p;

  plen = es.ES_enc28j60PacketReceive(BUFFER_SIZE, buf);

  /*plen will not equal zero if there is a valid packet (without crc error) */
  if (plen != 0) {

    // arp is broadcast if unknown but a host may also verify the mac address by sending it to a unicast address.
    if (es.ES_eth_type_is_arp_and_my_ip(buf, plen)) {
      DEBUG_PRINTLN("Send response to ARP request");
      es.ES_make_arp_answer_from_request(buf);
      return;
    }

    // check if packet is for us
    if (es.ES_eth_type_is_ip_and_my_ip(buf,plen) == 0) {
      DEBUG_PRINTLN("Packet is not for me");
      return;
    }
    
    if (buf[IP_PROTO_P] == IP_PROTO_ICMP_V && buf[ICMP_TYPE_P] == ICMP_TYPE_ECHOREQUEST_V) {
      DEBUG_PRINTLN("Sending ICMP response");
      es.ES_make_echo_reply_from_request(buf,plen);
      return;
    }
    
    // tcp port www start, compare only the lower byte
    if (buf[IP_PROTO_P] == IP_PROTO_TCP_V && buf[TCP_DST_PORT_H_P] == 0 && buf[TCP_DST_PORT_L_P] == port) {
      if (buf[TCP_FLAGS_P] & TCP_FLAGS_SYN_V) {
         DEBUG_PRINTLN("Received syn, sending synack");
         es.ES_make_tcp_synack_from_syn(buf); // make_tcp_synack_from_syn does already send the syn,ack
         return;     
      }
      if (buf[TCP_FLAGS_P] & TCP_FLAGS_ACK_V) {
        es.ES_init_len_info(buf); // init some data structures
        dat_p = es.ES_get_tcp_data_pointer();
        if (dat_p == 0) { // we can possibly have no data, just ack:
          if (buf[TCP_FLAGS_P] & TCP_FLAGS_FIN_V) {
            DEBUG_PRINTLN("Received no data, sending ack");
            es.ES_make_tcp_ack_from_any(buf);
          }
          return;
        }
        
        if (globalErrorBuffer[0]) {
          DEBUG_PRINTLN("Fatal error found");
          plen = es.ES_fill_tcp_data_p(buf, 0, PSTR("HTTP/1.0 500 OK\r\nContent-Type: application/json\r\n\r\n"));
          plen = addToBuffer(buf, plen, "{\"message\":\"");
          plen = addToBuffer(buf, plen, globalErrorBuffer);
          plen = addToBuffer(buf, plen, "\"}");
        } else {
          boolean managed = false;
          for (int i = 0; p_resource[i].id; i++) {
            int reslen = strlen(p_resource[i].method);
            if (strncmp(p_resource[i].method, (char *) & (buf[dat_p]), reslen) == 0
              && strncmp(p_resource[i].query, (char *) & (buf[dat_p + reslen]), strlen(p_resource[i].query)) == 0) {
                plen = p_resource[i].func(buf, dat_p + reslen, plen);
                managed = true;
                break;
            }
          }
          if (!managed) {
            plen = es.ES_fill_tcp_data_p(buf, 0, PSTR("HTTP/1.0 404 OK\r\nContent-Type: application/json\r\n\r\n{\"message\":\"404 No resource for this method & url\"}"));
          }
        }
        
        es.ES_make_tcp_ack_from_any(buf); // send ack for http get
        //DEBUG_HEXDUMP(buf, plen);
        es.ES_make_tcp_ack_with_data(buf, plen); // send data     
      }
    }
  }
}

/**
 * Fill buffer with default response
 */
uint16_t fillDefaultResponseBuffer(uint8_t *buf) {
  uint16_t plen = 0;

  plen = es.ES_fill_tcp_data_p(buf, plen, PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n"));
  plen = es.ES_fill_tcp_data_p(buf, plen, PSTR("{"));
  int flag = 0;
  for (int i = 0; i < p_pinSize; i++) {
    if (!(p_pin[i].mode == INPUT || p_pin[i].mode == ANALOG || p_pin[i].mode == DIGITAL)) {
      continue;
    }
    if (flag) {
      plen = es.ES_fill_tcp_data_p(buf, plen, PSTR(","));
    }
    
    plen = addToBuffer(buf, plen, i);
    plen = es.ES_fill_tcp_data_p(buf, plen, PSTR(":"));
    int sensorValue = getValue(i);
 
    plen = addToBuffer(buf, plen, sensorValue);
    flag++;
  }
  plen = es.ES_fill_tcp_data_p(buf, plen, PSTR("}"));
  return(plen);  
}


/**
 * Add a char * to buf buffer.
 * @return new position in buf
 */
uint16_t addToBuffer(uint8_t *buf, unsigned long pos, char *value, unsigned int len) {
  memcpy(&buf[TCP_CHECKSUM_L_P + 3 + pos], value, len);
  return pos + len;
}

/**
 * Add a char * to buf buffer.
 * @return new position in buf
 */
uint16_t addToBuffer(uint8_t *buf, unsigned long pos, char *value) {
  size_t size = strlen(value);
  memcpy(&buf[TCP_CHECKSUM_L_P + 3 + pos], value, size);
  return pos + size;
}

/**
 * Add a long char* representation to buf buffer.
 * @return new position in buf
 */
uint16_t addToBuffer(uint8_t *buf, unsigned long pos, uint16_t value) {
   static char value_to_add[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
   int j = 0;

   sprintf(value_to_add, "%d", value);
   while (value_to_add[j]) {
     buf[TCP_CHECKSUM_L_P + 3 + pos] = value_to_add[j++];
     pos++;
   }
   return pos;
}

/**
 * Add a long char* representation to buf buffer.
 * @return new position in buf
 */
uint16_t addToBufferHex(uint8_t *buf, unsigned long pos, uint16_t value) {
   static char value_to_add[3] = {0,0,0};
   int j = 0;

   sprintf(value_to_add, "%02X", value);
   while (value_to_add[j]) {
     buf[TCP_CHECKSUM_L_P + 3 + pos] = value_to_add[j++];
     pos++;
   }
   return pos;
}

/**
 * Start ethernet shield.
 */
void init28j60EthernetShield() {
  /*initialize enc28j60*/
  es.ES_enc28j60Init(mac);
  es.ES_enc28j60clkout(2); // change clkout from 6.25MHz to 12.5MHz
  delay(10);        
  /* Magjack leds configuration, see enc28j60 datasheet, page 11 */
  // LEDA=greed LEDB=yellow
  //
  // 0x880 is PHLCON LEDB=on, LEDA=on
  // enc28j60PhyWrite(PHLCON,0b0000 1000 1000 00 00);
  es.ES_enc28j60PhyWrite(PHLCON, 0x880);
  delay(500);
  // 0x990 is PHLCON LEDB=off, LEDA=off
  // enc28j60PhyWrite(PHLCON,0b0000 1001 1001 00 00);
  es.ES_enc28j60PhyWrite(PHLCON, 0x990);
  delay(500);
  // 0x880 is PHLCON LEDB=on, LEDA=on
  // enc28j60PhyWrite(PHLCON,0b0000 1000 1000 00 00);
  es.ES_enc28j60PhyWrite(PHLCON, 0x880);
  delay(500);
  // 0x990 is PHLCON LEDB=off, LEDA=off
  // enc28j60PhyWrite(PHLCON,0b0000 1001 1001 00 00);
  es.ES_enc28j60PhyWrite(PHLCON, 0x990);
  delay(500);
  // 0x476 is PHLCON LEDA=links status, LEDB=receive/transmit
  // enc28j60PhyWrite(PHLCON,0b0000 0100 0111 01 10);
  es.ES_enc28j60PhyWrite(PHLCON, 0x476);
  delay(100);        
  //init the ethernet/ip layer:
  es.ES_init_ip_arp_udp_tcp(mac, ip, port);
}

/**
 * init pins as input or output and set starting value
 */
void initPins() {
  for (int i = 0; i < p_pinSize; i++) {
    // skip notused and analog
    if (i >= 10 || p_pin[i].mode == NOTUSED) {
      continue;
    }

    // init input and output (PWM does not need init)
    if (p_pin[i].mode != PWM) {
      DEBUG_PRINT("set pin (");
      DEBUG_PRINTDEC(i);
      DEBUG_PRINT(") to mode ");
      
      DEBUG_PRINT((p_pin[i].mode == INPUT ? "INPUT" : (p_pin[i].mode == OUTPUT ? "OUTPUT" : "UNKNOWN")));
      DEBUG_PRINT(" (");
      DEBUG_PRINT(p_pin[i].description);
      DEBUG_PRINTLN(")");
      pinMode(i, p_pin[i].mode);
    }
    
    // set default value
    if (p_pin[i].mode == OUTPUT || p_pin[i].mode == PWM) {
      if (setValue(i, p_pin[i].startValue)) {
        // error when set
        DEBUG_PRINTLN(globalErrorBuffer);
        return;
      }
    } else if (p_pin[i].mode == INPUT && p_pin[i].startValue == HIGH) {
      DEBUG_PRINT("Enable 20k pullup resistor on pin ");
      DEBUG_PRINTDEC(i);
      DEBUG_PRINTLN("");
      digitalWrite(i, HIGH);
    }
  }
}

/**
 * Check pin definition in p_pin for this board version
 * @TODO duamilanov specific function
 */
int checkDef() {
  for (int i = 0; i < p_pinSize; i++) {
    // skip not used pin
    if (p_pin[i].mode == NOTUSED) {
      continue;
    }
    
    // check that for duemilanov the pin 10 to 13 are not used (used by ethernet shield)
    if (i >= 10 && i <= 13) {
      sprintf(globalErrorBuffer, "FATAL ERROR : Pin (%d) is used by ethernet shield you can not use it", i);
      return 1;
    }
    
    // check pwm pin
    if (p_pin[i].mode == PWM && !(i == 3 || i == 5 || i == 6 || i == 9 || i == 10 || i == 11)) {
      sprintf(globalErrorBuffer, "FATAL ERROR : Mode PWM can not be used for pin %d", i);
      return 1;
    }
    
    // check that analog pin is in ANALOG or DIGITAL
    if (i > 13 && !(p_pin[i].mode == ANALOG || p_pin[i].mode == DIGITAL)) {
      sprintf(globalErrorBuffer, "FATAL ERROR : Analog pin (%d) can only be in ANALOG or DIGITAL mode", i);
      return 1;
    }
    
    // check set default value for digital pins
    // skip digital INPUT pin check to allow 20k pullup start value (but no modification after that)
    if (i < 10 && (p_pin[i].mode == OUTPUT || p_pin[i].mode == PWM)) {
      // check that default value is applicable
      if (checkSetValue(i, p_pin[i].startValue)) {
        return 1;
      }
    }
  }
  return 0;
}

int getValue(int pin) {
  if (checkGetValue(pin)) {
    return 0;
  }
  if (p_pin[pin].mode == ANALOG) {
    return analogRead(pin);
  } else {
    return digitalRead(pin);
  }
}

/**
 * Check that pin is readable
 */
int checkGetValue(int pin) {
  if(p_pin[pin].mode == PWM) {
    sprintf(globalErrorBuffer, "You can not read this pin (%d) as its in PWM mode", pin);
    return 1;
  }
  return 0;
}

/**
 * check if value is setable and set it
 */
int setValue(int pin, int value) {
  if (checkSetValue(pin, value)) {
    return 1;
  }

  DEBUG_PRINT("set pin (");
  //DEBUG_PRINTDEC(pin);
  DEBUG_PRINT(" ");
  DEBUG_PRINT(p_pin[pin].mode == OUTPUT ? "OUTPUT" : "PWM");
  DEBUG_PRINT(") to value : ");
  DEBUG_PRINTDEC(value);
  DEBUG_PRINTLN("");
  
  if (p_pin[pin].mode == PWM) {
    analogWrite(pin, value);
  } else {
    digitalWrite(pin, value);
  }
  return 0;
}

/**
 * check that a value is setable to a pin
 * @return 1 if error
 * @TODO duamilanov specific function
 */
int checkSetValue(int pin, int value) {
  if (pin >= 10) {
    sprintf(globalErrorBuffer, "You can not set this pin (%d)", pin);
    return 1;
  }
  
  if (p_pin[pin].mode == PWM) {
    if (value < 0 || value > 255) {
      sprintf(globalErrorBuffer, "This value (%d) can not be set to PWM pin (%d)", value, pin);
      return 1;
    }
  } else if (p_pin[pin].mode == OUTPUT) {
    if (value < 0 || value > 1) {
      sprintf(globalErrorBuffer, "This value (%d) can not be set to OUTPUT pin (%d)", value, pin);
      return 1;
    }
  } else if (p_pin[pin].mode == INPUT) {
    if (value < 0 || value > 1) {
      sprintf(globalErrorBuffer, "This value (%d) can not be set to INPUT pin (%d)", value, pin);
      return 1;
    }
    // disable 20k pullup usage at run
    sprintf(globalErrorBuffer, "This pin (%d) is set as an input and can not be set", pin);
    return 1;
  } else {
    sprintf(globalErrorBuffer, "This pin (%d) is set as not used and can not be set", pin);
    return 1;    
  }
  return 0;
}



