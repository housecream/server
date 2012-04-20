#include "server-board.h"

uint16_t boardGet(char *buf, uint16_t dat_p, uint16_t plen) {
    plen = addToBufferTCP_P(buf, 0, HEADER_200);
    plen = addToBufferTCP_P(buf, plen, PSTR("{\"software\":\"HouseCream Client\", \"version\":\""));
    plen = addToBufferTCP_P(buf, plen, hcc_version);

    plen = addToBufferTCP_P(buf, plen, PSTR("\",\"hardware\":\""));
    plen = addToBufferTCP_P(buf, plen, PSTR(HARDWARE));

    plen = addToBufferTCP_P(buf, plen, PSTR("\",\"name\":\""));
    plen = addToBufferTCP_e(buf, plen, getConfigBoardName_E());

    plen = addToBufferTCP_P(buf, plen, PSTR("\",\"description\":\""));
    plen = addToBufferTCP_P(buf, plen, boardDescription.description);

    plen = addToBufferTCP_P(buf, plen, PSTR("\",\"notifyUrl\":\""));
    plen = addToBufferTCP(buf, plen, getConfigNotifyUrl());

    uint8_t ip[4];
    getConfigIP(ip);
    plen = addToBufferTCP_P(buf, plen, PSTR("\",\"ip\":\""));
    plen = addToBufferTCP(buf, plen, (uint16_t) ip[0]);
    plen = addToBufferTCP(buf, plen, '.');
    plen = addToBufferTCP(buf, plen, (uint16_t) ip[1]);
    plen = addToBufferTCP(buf, plen, '.');
    plen = addToBufferTCP(buf, plen, (uint16_t) ip[2]);
    plen = addToBufferTCP(buf, plen, '.');
    plen = addToBufferTCP(buf, plen, (uint16_t) ip[3]);

    plen = addToBufferTCP_P(buf, plen, PSTR("\",\"port\":"));
    plen = addToBufferTCP(buf, plen, getConfigPort());

    plen = addToBufferTCP_P(buf, plen, PSTR(",\"numberOfPin\":"));
    plen = addToBufferTCP(buf, plen, (uint16_t)NUMBER_OF_PINS);

    uint8_t mac[6];
    getConfigMac(mac);
    plen = addToBufferTCP_P(buf, plen, PSTR(",\"mac\":\""));
    plen = addToBufferTCPHex(buf, plen, mac[0]);
    plen = addToBufferTCP(buf, plen, ':');
    plen = addToBufferTCPHex(buf, plen, mac[1]);
    plen = addToBufferTCP(buf, plen, ':');
    plen = addToBufferTCPHex(buf, plen, mac[2]);
    plen = addToBufferTCP(buf, plen, ':');
    plen = addToBufferTCPHex(buf, plen, mac[3]);
    plen = addToBufferTCP(buf, plen, ':');
    plen = addToBufferTCPHex(buf, plen, mac[4]);
    plen = addToBufferTCP(buf, plen, ':');
    plen = addToBufferTCPHex(buf, plen, mac[5]);

    plen = addToBufferTCP_P(buf, plen, PSTR("\"}"));
    return plen;
}

uint16_t boardPut(char *buf, uint16_t dat_p, uint16_t plen) {
    char *data = &strstr(&buf[dat_p], DOUBLE_ENDL)[4];
    char *error = jsonParse(data, boardPutElements);
    if (error) {
        plen = addToBufferTCP_P(buf, 0, HEADER_400);
        plen = addToBufferTCP_P(buf, plen, PSTR("{\"message\":\""));
        plen = addToBufferTCP(buf, plen, error);
        plen = addToBufferTCP_P(buf, plen, PSTR("\"}"));
    } else {
        plen = addToBufferTCP_P(buf, 0, HEADER_200);
    }
    return plen;
}
