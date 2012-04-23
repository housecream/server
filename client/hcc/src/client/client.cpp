#include "client.h"

#define TCP_DATA_P          0x36

void clientNotify(int pinId, float oldValue, float value, t_notify notify) {
    notification = (t_notification *) malloc(sizeof(t_notification));
    notification->pinId = pinId;
    notification->value = value;
    notification->oldValue = oldValue;
    notification->notify.condition = notify.condition;
    notification->notify.value = notify.value;
}

uint16_t startRequestHeader(char **buf, const prog_char *method) {
    *buf = &((*buf)[TCP_DATA_P]);
    uint16_t plen;
    plen = addToBufferTCP_P(*buf, 0, method);
    return plen;
}

uint16_t clientBuildNextQuery(char *buf) {
    uint16_t plen;
    plen = startRequestHeader(&buf, PUT2);
    plen = addToBufferTCP_P(buf, plen, PSTR("/g"));
    plen = addToBufferTCP_P(buf, plen, PSTR(" HTTP/1.0\r\nAccept: application/json\r\nKeep-Alive: 300\r\nConnection: keep-alive\r\n\r\n"));
//    plen = addToBufferTCP_P2(buf, plen, PSTR("Host: 192.168.1.4\r\n"));
//    plen = addToBufferTCP_P2(buf, plen, PSTR("User-Agent: HouseCream Client\r\n"));

    plen = addToBufferTCP_P(buf, plen, PSTR("{id:"));
    plen = addToBufferTCP(buf, plen, notification->pinId);
    plen = addToBufferTCP_P(buf, plen, PSTR(",oldValue:"));
    plen = addToBufferTCP(buf, plen, notification->oldValue);
    plen = addToBufferTCP_P(buf, plen, PSTR(",value:"));
    plen = addToBufferTCP(buf, plen, notification->value);

    plen = addToBufferTCP_P(buf, plen, PSTR(",notify:{notifyCondition:\""));
    plen = addToBufferTCP_P(buf, plen, PSTR("ZZZZZZZZZZ"));
    plen = addToBufferTCP_P(buf, plen, PSTR("\",notifyValue:"));
    plen = addToBufferTCP(buf, plen, notification->notify.value);
    plen = addToBufferTCP_P(buf, plen, PSTR("}}"));
    return plen;
}
