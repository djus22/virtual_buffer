#include <iostream>
#include "packet.h"
#include <pcap.h>
#include <string>
#include <vector>
#include <iomanip>

#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>


using namespace std;
int main()
{
	string file = "plik_wejsciowy.pcap";


	char errbuff[PCAP_ERRBUF_SIZE];
	pcap_t * pcap = pcap_open_offline(file.c_str(), errbuff);
	struct pcap_pkthdr *header;

	const struct ether_header* ethernetHeader;
	const struct ip* ipHeader;
	const struct tcphdr* tcpHeader;


	const u_char *data;
	u_int packetCount = 0;

	vector<packet> in_tab;



	while (pcap_next_ex(pcap, &header, &data) >= 0)
		{

			int number = ++packetCount;

			static double const zero_time = double(header->ts.tv_sec) + (double(header->ts.tv_usec)/1000000);
			double time = double(header->ts.tv_sec) + (double(header->ts.tv_usec)/1000000) - zero_time;

			int dataLength = 0;
			ethernetHeader = (struct ether_header*)data;
			if(ntohs(ethernetHeader->ether_type) == ETHERTYPE_IP)
			{
				ipHeader = (struct ip*)(data + sizeof(struct ether_header));

				if(ipHeader->ip_p == IPPROTO_TCP)
				{
					tcpHeader = (tcphdr*)(data + sizeof(struct ether_header) + sizeof(struct ip));
					dataLength = header->len - (sizeof(struct ether_header) + sizeof(struct ip) + sizeof(struct tcphdr));

				}
			}

			int length = dataLength - 12;


			packet *pckt = new packet(number,time,length);
			in_tab.push_back(*pckt);

	    }

	for(int i = 0; i < in_tab.size() ; i++)
		in_tab[i].printPacket();



/*	cout << "************************* odplyw *********************************" << endl;

	file = "vodtest3rtp_odfiltrowany2.pcap";
	pcap = pcap_open_offline(file.c_str(), errbuff);

	vector<packet> out_tab;


	while (pcap_next_ex(pcap, &header, &data) >= 0)
		{

			int number = ++packetCount;
			double time = double(header->ts.tv_sec) + (double(header->ts.tv_usec)/1000000);

			int dataLength = 0;
			ethernetHeader = (struct ether_header*)data;
			if(ntohs(ethernetHeader->ether_type) == ETHERTYPE_IP)
			{
				ipHeader = (struct ip*)(data + sizeof(struct ether_header));

				if(ipHeader->ip_p == IPPROTO_TCP)
				{
					tcpHeader = (tcphdr*)(data + sizeof(struct ether_header) + sizeof(struct ip));
					dataLength = header->len - (sizeof(struct ether_header) + sizeof(struct ip) + sizeof(struct tcphdr));

				}
			}

			int length = dataLength - 12;


			packet *pckt = new packet(number,time,length);
			out_tab.push_back(*pckt);

	    }

	for(int i = 0; i < in_tab.size() ; i++)
		out_tab[i].printPacket();

*/





	return 0;
}
