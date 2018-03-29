#include <iostream>
#include "packet.h"
#include <pcap.h>
#include <string>
#include "input_packet.h"
#include <vector>
#include <iomanip>

using namespace std;
int main()
{
	string file = "plik_wejsciowy.pcap";


	char errbuff[PCAP_ERRBUF_SIZE];

	pcap_t * pcap = pcap_open_offline(file.c_str(), errbuff);

	struct pcap_pkthdr *header;


	const u_char *data;

	u_int packetCount = 0;

	vector<input_packet> in_tab;

	while (pcap_next_ex(pcap, &header, &data) >= 0)
	    {
	//        printf("Packet # %i", ++packetCount);
	//        printf("	Packet length: %5d bytes", header->len);

	        int number = ++packetCount;
	        double time = double(header->ts.tv_sec) + (double(header->ts.tv_usec)/1000000);
	        int length = header->len;

	        input_packet *pckt = new input_packet(number,time,length);
	     //   cout << (*pckt).getNumber();


	        in_tab.push_back(*pckt);

	        if (header->len != header->caplen)
	            printf("Warning! Capture size different than packet size: %ld bytes\n", header->len);

	 //       printf("	Timestamp: %d:%d seconds\n", header->ts.tv_sec, header->ts.tv_usec);


	    }

	for(int i = 0; i < in_tab.size() ; i++)
		in_tab[i].printPacket();



	return 0;
}
