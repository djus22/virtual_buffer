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

struct pes {
	unsigned char prefix[6];
	unsigned char stream_id[2];
	unsigned char len[2];
	unsigned char must_be_one :1;
	unsigned char must_be_zero :1;
	unsigned char scrambl :2;
	unsigned char prior_flag :1;
	unsigned char align_flag :1;
	unsigned char copyright_flag :1;
	unsigned char orig_flag :1;
	unsigned char pts_flag :1;
	unsigned char dts_flag :1;
	unsigned char escr_flag :1;
	unsigned char es_rate_flag :1;
	unsigned char dsm_trick_mode_flag :1;
	unsigned char add_copy_inf_flag :1;
	unsigned char crc_flag :1;
	unsigned char ext_flag :1;
	unsigned char data_len[2];


	unsigned char pts[5];
#if dts_flag == 1
	unsigned char dts[5];
	unsigned char* data = new unsigned char[16*len[0] + len[1] - 13];
#else
	unsigned char* data = new unsigned char[16*len[0] + len[1] - 8];
#endif


};




struct MpegTS
{

	unsigned char sync_byte :8;
	unsigned char transport_err :1;
	unsigned char payload_u_start_indictr :1;
	unsigned char transport_priority :1;
	unsigned char pid :1, :8, :4;
	unsigned char transport_scrmbl :2;
	unsigned char adapt_flag :2;
	unsigned char continuity_count :4;
#if adapt_flag == 0x3 		// adaptation field and payload
	unsigned char adapt_len;
	unsigned char discount_ind :1;
	unsigned char r_acces_ind :1;
	unsigned char elem_stream_prior_ind :1;
	unsigned char pcr_flag :1;
	unsigned char opcr_flag :1;
	unsigned char splicing_p_flag :1;
	unsigned char transp_priv_data_flag :1;
	unsigned char adapt_field_ext_flag :1;

	#if pcr_flag == 0x1;		// with PCR
		unsigned char pcr[6];
		unsigned char stuffing[(int)adapt_len - 2];
		unsigned char payload[184-((int)adapt_len + 11)];
	#else						// without PCR
		unsigned char stuffing [adapt_len - 1];
		unsigned char pointer;
		unsigned char payload [184-(int)adapt_len];

	#endif
#else						// if it's only main ts_header + payload
	unsigned char payload[184];
#endif


};


#endif /* DJ_HEADER_H_ */
