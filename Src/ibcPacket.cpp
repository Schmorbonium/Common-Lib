#include "ibcPacket.hpp"
#include "InterruptController.h"

#define IBCP_ATTN(header) ((IBCATTN)((header & 0xF000) >> 12))
#define IBCP_TTL(header) ((header & 0x0300) >> 8)
#define IBCP_LEN(header) ((header & 0x00E0) >> 5)
#define IBCP_ID(header) ((IBCID)((header & 0x001F) >> 0))

#define IBCP_PKT(attn, ttl, len, id) (((attn & 0xF) << 12) | ((ttl & 0x3) << 8) | ((len & 0x7) << 5)  | ((id & 0x1F) << 0))

IbcPacket::IbcPacket(CharBuffer* buf) {
    uint16_t header = (((uint16_t)buf->pop()) << 8) | buf->pop();

    this->attn = IBCP_ATTN(header);
    this->ttl = IBCP_TTL(header);
    this->len = IBCP_LEN(header);
    this->id = IBCP_ID(header);
    InterruptController_enter();
    this->data = new uint8_t[this->len];
    for (int i = len - 1; i >= 0; i--) {
        (this->data)[i] = buf->pop();
    }
    InterruptController_leave();
}


IbcPacket::IbcPacket(IBCATTN attn, uint8_t ttl, uint8_t len, IBCID id, uint8_t* data) {
    this->attn = attn;
    this->ttl = ttl;
    this->len = len;
    this->id = id;
    this->data = new uint8_t[this->len];
    for (int i = len - 1; i >= 0; i--) {
        (this->data)[i] = data[i];
    }
}

IbcPacket::~IbcPacket() {
    InterruptController_enter();
    delete this->data;
    InterruptController_leave();
}

void IbcPacket::queueInto(CharBuffer* buf) {
    // reconstruct header into dense format and queue its 2 bytes
    uint16_t header = IBCP_PKT(this->attn, this->ttl, this->len, this->id);
    buf->append((header & 0xFF00) >> 8);
    buf->append(header & 0xFF);
    // queue every byte of data
    for (int i = len - 1; i >= 0; i--) {
        buf->append(this->data[i]);
    }
}
