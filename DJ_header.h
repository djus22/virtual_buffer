/*
 * DJ_header.h
 *
 *  Created on: Apr 21, 2018
 *      Author: dawid
 */

#ifndef DJ_HEADER_H_
#define DJ_HEADER_H_

#include <iostream>
#include "packet.h"
#include <pcap.h>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <algorithm>
#include "Buffer.h"





struct rtpHeader {
#if __BYTE_ORDER == __BIG_ENDIAN
        //For big endian
        unsigned char version:2;       // Version, currently 2
        unsigned char padding:1;       // Padding bit
        unsigned char extension:1;     // Extension bit
        unsigned char cc:4;            // CSRC count
        unsigned char marker:1;        // Marker bit
        unsigned char payload:7;       // Payload type
#else
        //For little endian
        unsigned char cc:4;            // CSRC count
        unsigned char extension:1;     // Extension bit
        unsigned char padding:1;       // Padding bit
        unsigned char version:2;       // Version, currently 2
        unsigned char payload:7;       // Payload type
        unsigned char marker:1;        // Marker bit
#endif
        u_int16_t sequence;        // sequence number
        u_int32_t timestamp;       //  timestamp
        u_int32_t sources[1];      // contributing sources
};


#endif /* DJ_HEADER_H_ */
