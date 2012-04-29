#include "server-pin.h"

uint16_t pinPutValue(char *buf, uint16_t dat_p, uint16_t plen, t_webRequest *webResource) {
//    int t = atoi(&buf[dat_p]);
//    defaultPinWrite(pinId, t);
//    DEBUG_P(PSTR("pvalue"));
//    DEBUG_PRINTLN(pinId);
//    DEBUG_PRINTLN(t);
    plen = startResponseHeader(&buf, HEADER_200);
    return plen;
}

uint16_t pinGetValue(char *buf, uint16_t dat_p, uint16_t plen, t_webRequest *webResource) {
    plen = startResponseHeader(&buf, HEADER_200);
    plen = addToBufferTCP_P(buf, plen, PSTR("pinGetValue"));
    return plen;
}

uint16_t pinPut(char *buf, uint16_t dat_p, uint16_t plen, t_webRequest *webResource) {
    currentSetPinIdx = webResource->pinIdx;
    const prog_char *error = jsonParse(&buf[dat_p], &pinPutObj);
    if (error) {
        plen = startResponseHeader(&buf, HEADER_400);
        plen = appendErrorMsg_P(buf, plen, error);
    } else {
        plen = startResponseHeader(&buf, HEADER_200);
    }
    return plen;
}

uint16_t pinGet(char *buf, uint16_t dat_p, uint16_t plen, t_webRequest *webResource) {
    plen = startResponseHeader(&buf, HEADER_200);
    plen = addToBufferTCP_P(buf, plen, PSTR("{\"id\":"));
    plen = addToBufferTCP(buf, plen, (uint16_t) pgm_read_byte(webResource->pinIdx < pinInputSize ?
            &pinInputDescription[webResource->pinIdx].pinId : &pinOutputDescription[webResource->pinIdx - pinInputSize].pinId));


    plen = addToBufferTCP_P(buf, plen, PSTR(",\"name\":\""));
    plen = addToBufferTCP_E(buf, plen, getConfigPinName_E(webResource->pinIdx));

    plen = addToBufferTCP_P(buf, plen, PSTR("\",\"description\":\""));
    plen = addToBufferTCP_P(buf, plen, (const prog_char *) (webResource->pinIdx < pinInputSize ?
            &pinInputDescription[webResource->pinIdx].description : &pinOutputDescription[webResource->pinIdx - pinInputSize].description));

    plen = addToBufferTCP_P(buf, plen, PSTR("\",\"direction\":\""));
    plen = addToBufferTCP_P(buf, plen, webResource->pinIdx < pinInputSize ? STR_INPUT : STR_OUTPUT);
    plen = addToBufferTCP(buf, plen, '"');

    uint8_t type = pgm_read_byte(webResource->pinIdx < pinInputSize ?
            &pinInputDescription[webResource->pinIdx].type : &pinOutputDescription[webResource->pinIdx - pinInputSize].type);
    plen = addToBufferTCP_P(buf, plen, PSTR(",\"type\":\""));
    plen = addToBufferTCP_P(buf, plen, (const prog_char *) pgm_read_byte(&pinType[type - 1]));

    if (webResource->pinIdx < pinInputSize) {
        PinInputConversion conversion = (PinInputConversion) pgm_read_word(&(pinInputDescription[webResource->pinIdx].convertValue));

        plen = addToBufferTCP_P(buf, plen, PIN_MIN);
        plen = addToBufferTCP(buf, plen, conversion(0));

        plen = addToBufferTCP_P(buf, plen, PIN_MAX);
        plen = addToBufferTCP(buf, plen, conversion(type == ANALOG ? 1023 : 1));

        plen = addToBufferTCP_P(buf, plen, PSTR(",\"notifies\":["));
        for (uint8_t i = 0; i < PIN_NUMBER_OF_NOTIFY; i++) {
            t_notify notify;
            getConfigPinNotify(webResource->pinIdx, i, &notify);
            if (notify.condition > 0) {
                if (i) {
                    plen = addToBufferTCP(buf, plen, ',');
                }
                plen = addToBufferTCP_P(buf, plen, PSTR("{\"notifyCondition\":\""));
                plen = addToBufferTCP_P(buf, plen, (const prog_char *) pgm_read_byte(&pinNotification[notify.condition - 1]));

                plen = addToBufferTCP_P(buf, plen, PSTR("\",\"notifyValue\":"));
                plen = addToBufferTCP(buf, plen, notify.value);
                plen = addToBufferTCP(buf, plen, '}');
            }
        }
        plen = addToBufferTCP(buf, plen, ']');
    } else {
        float minValue;
        memcpy_P(&minValue, &pinOutputDescription[webResource->pinIdx - pinInputSize].valueMin, sizeof(float));
        plen = addToBufferTCP_P(buf, plen, PIN_MIN);
        plen = addToBufferTCP(buf, plen, minValue);

        float maxValue;
        memcpy_P(&maxValue, &pinOutputDescription[webResource->pinIdx - pinInputSize].valueMax, sizeof(float));
        plen = addToBufferTCP_P(buf, plen, PIN_MAX);
        plen = addToBufferTCP(buf, plen, maxValue);
    }
//
//        plen = addToBufferTCP_P(buf, plen, PSTR(",\"value\":"));
//        plen = addToBufferTCP(buf, plen, getPinValue(pinId));
    plen = addToBufferTCP(buf, plen, '}');
    return plen;
}


