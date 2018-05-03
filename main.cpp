
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


//***********************************************************************************************************************

	file = "vodtest3rtp_odfiltrowany2.pcap";
	pcap = pcap_open_offline(file.c_str(), errbuff);

	vector<packet> out_tab;


	fstream plik_hex("hex.txt" , ios::out | ios::in);

	packetCount = 0;
	bool pts_zero_flag = true;
	bool dts_zero_flag = true;
	unsigned long long pts_zero;
	unsigned long long dts_zero;


	while (pcap_next_ex(pcap, &header, &data) >= 0)
		{


			double time ;
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

							short counter = 0;
							for(u_int i = offset ; i < header->caplen ; i++, counter++)
								{
									if((counter % 188) == 0)
									{
										plik_hex << "\n";
										counter = 0;
									}
									bool isPES;
									if(counter == 4 || static_cast<int>(data[i-1]) == 0xff)
										isPES = true;
									else
										isPES = false;

									if(isPES && static_cast<int>(data[i]) == 0 && static_cast<int>(data[i+1]) == 0 && static_cast<int>(data[i+2]) == 1)
									{
										int number = ++packetCount;
										int l_byte = static_cast<int>(data[i+4]);
										int r_byte = static_cast<int>(data[i+5]);
										dataLength = r_byte + ((l_byte - l_byte%16) / 16)*4096 + (l_byte%16)*256 + 6;

										if(static_cast<int>(data[i+7]) >= 192) // PTS and DTS both
										{
											int byte_4 = static_cast<int>(data[i+14]);
											int byte_3 = static_cast<int>(data[i+15]);
											int byte_2 = static_cast<int>(data[i+16]);
											int byte_1 = static_cast<int>(data[i+17]);
											int byte_0 = static_cast<int>(data[i+18]);



											uint64_t v = ((byte_4 - byte_4%16) / 16)*68719476736 + (byte_4%16)*4294967296
													+ ((byte_3 - byte_3%16) / 16)*268435456 + (byte_3%16)*16777216
													+ ((byte_2 - byte_2%16) / 16)*1048576 + (byte_2%16)*65536
													+ ((byte_1 - byte_1%16) / 16)*4096 + (byte_1%16)*256
													+ byte_0 ;
											uint64_t dts = 0;
											dts |= (v >> 3) & (0x0007 << 30); // top 3 bits, shifted left by 3, other bits zeroed out
											dts |= (v >> 2) & (0x7fff << 15); // middle 15 bits
											dts |= (v >> 1) & (0x7fff <<  0); // bottom 15 bits


											if(dts_zero_flag)
											{
												dts_zero = dts;
												dts_zero_flag = false;
											}



											time = ((double(dts - dts_zero)) / 90000);
											cout << setprecision(11) << time << "\n";
										}
										else if (static_cast<int>(data[i+7]) >= 128)// only PTS
										{
											int byte_4 = static_cast<int>(data[i+9]);
											int byte_3 = static_cast<int>(data[i+10]);
											int byte_2 = static_cast<int>(data[i+11]);
											int byte_1 = static_cast<int>(data[i+12]);
											int byte_0 = static_cast<int>(data[i+13]);
											uint64_t v = ((byte_4 - byte_4%16) / 16)*68719476736 + (byte_4%16)*4294967296
													+ ((byte_3 - byte_3%16) / 16)*268435456 + (byte_3%16)*16777216
													+ ((byte_2 - byte_2%16) / 16)*1048576 + (byte_2%16)*65536
													+ ((byte_1 - byte_1%16) / 16)*4096 + (byte_1%16)*256
													+ byte_0 ;

											uint64_t pts = 0;
											pts |= (v >> 3) & (0x0007 << 30); // top 3 bits, shifted left by 3, other bits zeroed out
											pts |= (v >> 2) & (0x7fff << 15); // middle 15 bits
											pts |= (v >> 1) & (0x7fff <<  0); // bottom 15 bits

											if(pts_zero_flag)
											{
												pts_zero = pts;
												pts_zero_flag = false;
											}
											time = (double(pts - pts_zero)) / 90000;
											cout << setprecision(11) << time << "\n";
										}

										packet *pckt = new packet(number,time,dataLength);
										out_tab.push_back(*pckt);

									}

								plik_hex << hex << setfill('0') << setw(2) << static_cast<int>(data[i]) << " ";
								}
								plik_hex << "\n ";
						}

				}
			}

	    }



//	sort(out_tab.begin() , out_tab.end());

	fstream plik_out("wyjscie.txt" , ios::out);

	if(plik_out.good())
	{
		for(u_int i = 0; i < out_tab.size() ; i++)
		{
			plik_out << "packet number: " << out_tab[i].getNumber() << "	";
			plik_out << "packet time: " << setprecision(8) << setw(11) << out_tab[i].getTime() << " seconds" << "			";
			plik_out << "packet length: "  << setw(8) << out_tab[i].getLength() << " bytes" << "\n";

		}
	}
	plik_out.close();
	plik_hex.close();






	return 0;
}
