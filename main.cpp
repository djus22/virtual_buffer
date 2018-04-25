
#include "DJ_header.h"



using namespace std;
int main()
{
	string file = "plik_wejsciowy.pcap";


	char errbuff[PCAP_ERRBUF_SIZE];
	pcap_t * pcap = pcap_open_offline(file.c_str(), errbuff);
	struct pcap_pkthdr *header;

	const struct ether_header* ethernetHeader;
	const struct ip* ipHeader;
	const struct rtpHeader* rtpHdr;
	const struct udphdr* udpHeader;
	const struct pes* pes;


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
					dataLength = header->len - (sizeof(struct ether_header) + sizeof(struct ip) + sizeof(struct tcphdr));

				}
			}

			int length = dataLength - 12;
			if(number == 1)
				{
					length -= 8;
				}


			packet *pckt = new packet(number,time,length);
			in_tab.push_back(*pckt);


	    }

	fstream plik_in("wejcie.txt" , ios::out );

	if(plik_in.good())
	{
		for(u_int i = 0; i < in_tab.size() ; i++)
		{
			plik_in << "packet number: " << in_tab[i].getNumber() << "	";
			plik_in << "packet time: " << setprecision(16) << setw(18) << in_tab[i].getTime() << " seconds" << "		";
			plik_in << "packet length: "  << setw(8) << in_tab[i].getLength() << " bytes" << "\n";

		}
	}
	plik_in.close();


	cout << "************************* odplyw *********************************" << endl;

	file = "vodtest3rtp_odfiltrowany2.pcap";
	pcap = pcap_open_offline(file.c_str(), errbuff);

	vector<packet> out_tab;


	fstream plik_hex("hex.txt" , ios::out | ios::in);

	packetCount = 0;

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

				if(ipHeader->ip_p == IPPROTO_UDP)
				{
					udpHeader = (struct udphdr*)(data + sizeof(struct ip));
					rtpHdr = (struct rtpHeader*)(data + sizeof(struct udphdr));


					int offset = sizeof(struct ether_header) + sizeof(struct ip) + sizeof(struct rtpHeader) + sizeof(struct udphdr);
					if(plik_hex.good())
						{
							for(u_int i = offset ; i < header->caplen ; i++)
								{
									if((i%16) == 0)
										plik_hex << "\n";

									plik_hex << hex << setfill('0') << setw(2) << static_cast<int>(data[i]) << " ";
								}
								plik_hex << "\n " << "\n";
						}

				}
			}




			packet *pckt = new packet(number,time,dataLength);
			out_tab.push_back(*pckt);

	    }





	cout << "GAME OVER";

	fstream plik_out("wyjscie.txt" , ios::out);

	if(plik_out.good())
	{
		for(u_int i = 0; i < out_tab.size() ; i++)
		{
			plik_out << "packet number: " << out_tab[i].getNumber() << "	";
			plik_out << "packet time: " << setprecision(16) << setw(18) << out_tab[i].getTime() << " seconds" << "		";
			plik_out << "packet length: "  << setw(8) << out_tab[i].getLength() << " bytes" << "\n";

		}
	}
	plik_out.close();
	plik_hex.close();






	return 0;
}
