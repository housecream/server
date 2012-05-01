#include "client.h"

#define TCP_DATA_P          0x36

void clientNotify(int pinId, float oldValue, float value, t_notify *notify) {
    DEBUG_PRINT("notify pin");
    DEBUG_PRINTLN(pinId);
    t_notification *tmpNotification = (t_notification *) malloc(sizeof(t_notification));
    tmpNotification->pinId = pinId;
    tmpNotification->value = value;
    tmpNotification->oldValue = oldValue;
    tmpNotification->notify.condition = notify->condition;
    tmpNotification->notify.value = notify->value;
    tmpNotification->next = 0;


    if(notification == 0) {
        notification = tmpNotification;
    } else {
        t_notification* temp=notification;
        uint8_t size = 0;
        while(temp->next != 0) {
            temp = temp->next;
            size++;
        }
        if (size > 10) {
            free(tmpNotification);
        } else {
            temp->next = tmpNotification;
        }
    }
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
    uint16_t start = plen;
    plen = addToBufferTCP(buf, plen, notifyUrlPrefix);
    plen = addToBufferTCP_P(buf, plen, PSTR("/notify/pin"));
    plen = addToBufferTCP_P(buf, plen, PSTR(" HTTP/1.0\r\nContent-Type: application/json\r\nKeep-Alive: 300\r\nConnection: keep-alive\r\n"));
//    plen = addToBufferTCP_P2(buf, plen, PSTR("Host: 192.168.1.4\r\n"));
//    plen = addToBufferTCP_P2(buf, plen, PSTR("User-Agent: HouseCream Client\r\n"));
    plen = addToBufferTCP_P(buf, plen, PSTR("Content-Length: ???\r\n\r\n"));

    uint16_t datapos = plen;
    plen = addToBufferTCP_P(buf, plen, PSTR("{\"id\":"));
    plen = addToBufferTCP(buf, plen, (uint16_t) notification->pinId);
    plen = addToBufferTCP_P(buf, plen, PSTR(",\"oldValue\":"));
    plen = addToBufferTCP(buf, plen, notification->oldValue);
    plen = addToBufferTCP_P(buf, plen, PSTR(",\"value\":"));
    plen = addToBufferTCP(buf, plen, notification->value);

    plen = addToBufferTCP_P(buf, plen, PSTR(",\"notify\":{\"notifyCondition\":\""));
    plen = addToBufferTCP_P(buf, plen, (const prog_char *) pgm_read_byte(&pinNotification[notification->notify.condition - 1]));
    plen = addToBufferTCP_P(buf, plen, PSTR("\",\"notifyValue\":"));
    plen = addToBufferTCP(buf, plen, notification->notify.value);
    plen = addToBufferTCP_P(buf, plen, PSTR("}}"));
    itoa(plen - datapos, &buf[datapos - 7], 10);
    buf[datapos - 7 + 3] = '\r';

    // free this notification
    t_notification *next = notification->next;
    free(notification);
    notification = next;

    return plen;
}
