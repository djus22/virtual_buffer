
#include "DJ_header.h"


using namespace std;
int main()
{
//	string file = "plik_wejsciowy.pcap";
//	string file = "plik_in.pcap";
//	string file = "test_mp4_over_http.pcap";
	string file = "test_mp4_over_http_server_v3.pcap";


	char errbuff[PCAP_ERRBUF_SIZE];
	pcap_t * pcap = pcap_open_offline(file.c_str(), errbuff);
	struct pcap_pkthdr *header;

	const struct ether_header* ethernetHeader;
	const struct ip* ipHeader;
	const struct tcphdr* tcpHeader;

	const u_char *data;
	u_int packetCount = 0;

	vector<packet> in_tab;
	set<long long int> s;
	int *in_bytes = new int{0};
	bool isHttp{false};

	while (pcap_next_ex(pcap, &header, &data) >= 0)
		{

			int number;
			long long int seq_nr;


			// setting time for packet
			static double const zero_time = double(header->ts.tv_sec) + (double(header->ts.tv_usec)/1000000);
			double time = double(header->ts.tv_sec) + (double(header->ts.tv_usec)/1000000) - zero_time;

			int dataLength = 0;
			ethernetHeader = (struct ether_header*)data;
			if(ntohs(ethernetHeader->ether_type) == ETHERTYPE_IP)
			{
				ipHeader = (struct ip*)(data + sizeof(struct ether_header));

				if(ipHeader->ip_p == IPPROTO_TCP)
				{
					int offset = (sizeof(struct ether_header) + sizeof(struct ip) + sizeof(struct tcphdr));

					tcpHeader = (tcphdr*)(data + sizeof(struct ether_header) + sizeof(struct ip));
					seq_nr = tcpHeader->th_seq;

					number = ++packetCount;

/*
//##################################################################################################################################################################################################################################################################################################################################################################################################################################################################
					// checking if packet is a HTTP header
					for(u_int i = offset ; i < header->len ; i++)
					{
						if(static_cast<int>(data[i]) == 'H' && static_cast<int>(data[i+1]) == 'T' && static_cast<int>(data[i+2]) == 'T' && static_cast<int>(data[i+3]) == 'P')
								isHttp = true;

						if(static_cast<int>(data[i]) == 'f' && static_cast<int>(data[i+1]) == 'i' && static_cast<int>(data[i+2]) == 'l' && static_cast<int>(data[i+3]) == 'e'
							&& static_cast<int>(data[i+4]) == 's' && static_cast<int>(data[i+5]) == 'i' && static_cast<int>(data[i+6]) == 'z' && static_cast<int>(data[i+7]) == 'e')
						{
							isHttp = false;
							offset = i+7;
						}

					}
//####################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################################
*/

					//calculating paylod size (packet reduced by ether, ip, tcp and optional http headers)
					dataLength = header->len - offset;


				}
			}


			int length = dataLength - 12;
			if(number == 1)
				{
					length -= 8;
				}

			// adding not retransmitted packets only
			if(insertInSet(s , seq_nr))
			{
				if(length && !isHttp) // adding only not empty packets without http header
				{
					*in_bytes += length;
					packet *pckt = new packet(number,time,length);
					in_tab.push_back(*pckt);

				}
			}

	    }


	s.clear();


	// writing stats of packet to file
	fstream plik_in("wejscie.txt" , ios::out );
	if(plik_in.good())
	{
		for(u_int i = 0; i < in_tab.size() ; i++)
		{
			plik_in << "pakiet numer: " << setw(4) << in_tab[i].getNumber() << "	";
			plik_in << "czas: " << setprecision(8) << setw(11) << in_tab[i].getTime() << " sekund" << "	";
			plik_in << "dlugosc pakietu: "  << setw(8) << in_tab[i].getLength() << " bajtow" << "\n";

		}
	}
	plik_in.close();
	cout << "wczytywanie pliku wejsciowego: OK\n";


//***********************************************************************************************************************

//	file = "vodtest3rtp_odfiltrowany2.pcap";
	file = "plik_out.pcap";

	pcap = pcap_open_offline(file.c_str(), errbuff);

	vector<packet> out_tab;
	int *out_bytes = new int{0};
	int *stamp = new int{-1};

	fstream plik_hex("hex.txt" , ios::out | ios::in);


	packetCount = 0;

	unsigned long long pts_zero{0};		// lowest PTS of packets with only PTS
	unsigned long long dts_zero{0};		// lowest DTS of packets with PTS and DTS
	unsigned long long pts_dts_zero{0};	// lowest PTS of packets with PTS and DTS

	cout << "Prosze podac znacznik czasu wg ktorego oznaczac pakiety wyjsciowe:\n"
		<< "0. PTS\n" << "1. DTS\n";

	while(*stamp != 1 && *stamp != 0)
		{
			cin >> *stamp;
			if(*stamp != 1 && *stamp != 0)
				cout << "Podales zle wartosci\n";
		}



	while (pcap_next_ex(pcap, &header, &data) >= 0)
		{


			double time ;
			ethernetHeader = (struct ether_header*)data;
			if(ntohs(ethernetHeader->ether_type) == ETHERTYPE_IP)
			{
				ipHeader = (struct ip*)(data + sizeof(struct ether_header));

				if(ipHeader->ip_p == IPPROTO_UDP)
				{

					if(plik_hex.good())
					{


					// skipping headers: ether, ip, rtp and udp
					int offset = sizeof(struct ether_header) + sizeof(struct ip) + sizeof(struct rtpHeader) + sizeof(struct udphdr);



					short counter = 0;
					for(u_int i = offset ; i < header->caplen ; i++, counter++)
						{


							// reading data as 188 bytes parts (TS packets)
							if( !(counter % 188) )
								{
									plik_hex << "\n";
									counter = 0;
								}

							static int payload = 0;
							int stuffing = 0;


							// looking for flag of payload only at TS headers
							if(counter == 0)
							{
								if(static_cast<int>(data[i+3]) <= 31 || (static_cast<int>(data[i+3]) >= 208 && static_cast<int>(data[i+3]) <= 223) )
								{
									payload += 184;

								}
								// if payload exists with stuffing bytes
								else
								{
									stuffing = static_cast<int>(data[i+4]);
									payload += 183 - stuffing;

								}


							}



							plik_hex << hex << setfill('0') << setw(2) << static_cast<int>(data[i]) << " ";
							bool isPES;
							// byte just behind TSheader or stuffing bytes => isPES = true
							if(counter == 4 || static_cast<int>(data[i-1]) == 0xff)
								isPES = true;
							else
								isPES = false;

							// looking for PES prefix (00 00 01)
							if(isPES && static_cast<int>(data[i]) == 0
									&& static_cast<int>(data[i+1]) == 0
									&& static_cast<int>(data[i+2]) == 1)
								{
									int number = ++packetCount;



									// when pts-flag == dts-flag == 1 then 7th field from prefix >= 192
									if(static_cast<int>(data[i+7]) >= 192) // PTS and DTS both in PES Header Data
										{
											if(*stamp == 1) // sorting by DTS
												{
													// reading each byte of DTS from bitfields
													int byte_4 = static_cast<int>(data[i+14]);
													int byte_3 = static_cast<int>(data[i+15]);
													int byte_2 = static_cast<int>(data[i+16]);
													int byte_1 = static_cast<int>(data[i+17]);
													int byte_0 = static_cast<int>(data[i+18]);


													// calculating DTS
													uint64_t v = ((byte_4 - byte_4%16) / 16)*68719476736 + (byte_4%16)*4294967296
															+ ((byte_3 - byte_3%16) / 16)*268435456 + (byte_3%16)*16777216
															+ ((byte_2 - byte_2%16) / 16)*1048576 + (byte_2%16)*65536
															+ ((byte_1 - byte_1%16) / 16)*4096 + (byte_1%16)*256
															+ byte_0 ;

													uint64_t dts = 0;
													dts |= (v >> 3) & (0x0007 << 30); // top 3 bits, shifted left by 3, other bits zeroed out
													dts |= (v >> 2) & (0x7fff << 15); // middle 15 bits
													dts |= (v >> 1) & (0x7fff <<  0); // bottom 15 bits


													if(dts_zero == 0)
													{
														dts_zero = dts;
													}
													else
													{	// looking for the lowest pts_dts
														if(dts_zero > dts)
															dts = dts_zero;
													}


													// DTS have units of 1/90000 second
													time = double(dts) / 90000;

													*out_bytes += payload;
													packet *pckt = new packet(number,time,payload);
													out_tab.push_back(*pckt);
													payload = 0;

												}
											else 	// sorting by PTS
												{
													// reading each byte of PTS from bitfields
													int byte_4 = static_cast<int>(data[i+9]);
													int byte_3 = static_cast<int>(data[i+10]);
													int byte_2 = static_cast<int>(data[i+11]);
													int byte_1 = static_cast<int>(data[i+12]);
													int byte_0 = static_cast<int>(data[i+13]);



													// calculating PTS
													uint64_t v = ((byte_4 - byte_4%16) / 16)*68719476736 + (byte_4%16)*4294967296
															+ ((byte_3 - byte_3%16) / 16)*268435456 + (byte_3%16)*16777216
															+ ((byte_2 - byte_2%16) / 16)*1048576 + (byte_2%16)*65536
															+ ((byte_1 - byte_1%16) / 16)*4096 + (byte_1%16)*256
															+ byte_0 ;

													uint64_t pts_dts = 0;
													pts_dts |= (v >> 3) & (0x0007 << 30); // top 3 bits, shifted left by 3, other bits zeroed out
													pts_dts |= (v >> 2) & (0x7fff << 15); // middle 15 bits
													pts_dts |= (v >> 1) & (0x7fff <<  0); // bottom 15 bits

													if(pts_dts_zero == 0)
													{
														pts_dts_zero = pts_dts;
													}
													else
													{	// looking for the lowest pts_dts
														if(pts_dts_zero > pts_dts)
															pts_dts = pts_dts_zero;
													}

													// PTS have units of 1/90000 second
													time = double(pts_dts) / 90000;

													*out_bytes += payload;
													packet *pckt = new packet(number,time,payload);
													out_tab.push_back(*pckt);
													payload = 0;
												}
										}

									// when pts-flag == 1 and dts-flag == 0 then 7th field from prefix >= 128
									else if (static_cast<int>(data[i+7]) >= 128 && *stamp == 0)// only PTS in PES Header Data
										{
											// reading each byte of PTS from bitfields
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

											// calculating PTS
											uint64_t pts = 0;
											pts |= (v >> 3) & (0x0007 << 30); // top 3 bits, shifted left by 3, other bits zeroed out
											pts |= (v >> 2) & (0x7fff << 15); // middle 15 bits
											pts |= (v >> 1) & (0x7fff <<  0); // bottom 15 bits


											if(pts_zero == 0)
											{
												pts_zero = pts;
											}
											else
											{	// looking for the lowest pts
												if(pts_zero > pts)
												pts = pts_zero;
											}
											// PTS have units of 1/90000 second
											time = double(pts) / 90000;

											*out_bytes += payload;
											packet *pckt = new packet(number,time,payload);
											out_tab.push_back(*pckt);
											payload = 0;
										}


								}

						}


					}
				}
			}

	    }


	sort(out_tab.begin() , out_tab.end() , packet::sortByTime);


	double p_zero = ((double)pts_zero) / 90000;
	double pd_zero = ((double)pts_dts_zero) / 90000;
	double d_zero = ((double)dts_zero) / 90000;

//changing raw-times of packets to delta-times


	// when sorted by dts
	for(u_int i = 0 ; i < out_tab.size() ; i++)
	{
		out_tab[i].setTime(out_tab[i].getTime() - d_zero);

	}

	// when sorted by pts's
	if(p_zero < pd_zero)
		{
			u_int i = 0;
			for( ; out_tab[i].getTime() < pd_zero ; i++)
			{
				out_tab[i].setTime(out_tab[i].getTime() - p_zero);
			}

			for( ; i < out_tab.size() ; i++)
			{
				out_tab[i].setTime(out_tab[i].getTime() - pd_zero);

			}

		}
	else
		{
			u_int i = 0;
			for( ; out_tab[i].getTime() < p_zero ; i++)
			{
				out_tab[i].setTime(out_tab[i].getTime() - pd_zero);
			}


			for( ; i < out_tab.size() ; i++)
			{
				out_tab[i].setTime(out_tab[i].getTime() - p_zero);
			}

		}

	sort(out_tab.begin() , out_tab.end() , packet::sortByTime);



	double delta = 0;
	cout << "Prosze podac opoznienie wczytywania pliku wyjsciowego w sekundach: ";
	cin >> delta;

	for(u_int i = 0; i < out_tab.size() ; i++)
	{
		out_tab[i].setTime(out_tab[i].getTime() + delta);
	}


	fstream plik_out("wyjscie.txt" , ios::out);

	if(plik_out.good())
	{
		for(u_int i = 0; i < out_tab.size() ; i++)
		{
			plik_out << "pakiet numer: " << setw(4) << out_tab[i].getNumber() << "	";
			plik_out << "czas: " << setprecision(8) << setw(11) << out_tab[i].getTime() << " sekund" << "	";
			plik_out << "dlugosc pakietu: "  << setw(8) << out_tab[i].getLength() << " bajtow" << "\n";

		}
	}

	cout << "wczytywanie pliku wyjsciowego: OK\n";

	fstream stat("statistics.txt" , ios::out);
	if(stat.good())
	{

		Buffer *buff = new Buffer();
		u_int in = 0 , out = 0;


		for(;;)
		{
			if((in_tab[in].getTime() <= out_tab[out].getTime() && in != in_tab.size()) || out == out_tab.size())
			{
				stat <<"czas: " << setprecision(8) << setw(11) << in_tab[in].getTime() <<
						" - dodano " << in_tab[in].getLength() << " bajtow danych\n";
				if(in_tab[in].getLength() > 0)
					buff->incrSize(&in_tab[in]);
				if(in < in_tab.size())
				{
					in++;
				}
			}
			else
			{
				if(buff->isEmpty())
				{

					buff->incrEmptyCounter();
					if(buff->getEmptySince() == -999999999)
						buff->setEmptySince(&out_tab[out]);
					stat <<"czas: " << setprecision(8) << setw(11) << out_tab[out].getTime() << " - nieudana proba pobrania danych\n";
				}
				else
				{
					int dwnld = (out_tab[out].getLength() > buff->getSize()) ? buff->getSize() : out_tab[out].getLength() ;
					buff->decrSize(&out_tab[out]);
					stat << "czas: " << setprecision(8) << setw(11) << out_tab[out].getTime() << " - pobrano " << dwnld << " bajtow danych\n";
				}
				if(out < out_tab.size())
				{
					out++;
				}

			}

			if(in == in_tab.size() && out == out_tab.size())
			{
				if(in_tab[in-1].getTime() < out_tab[out-1].getTime())
				{
					buff->incrEmptyTime(&out_tab[out-1]);
				}
				break;
			}
		}

		stat << "liczba nieudanych prob pobrania danych z bufora: " << buff->getEmptyCounter() << "\n";
		stat << "bufor byl pusty lacznie przez: " << setprecision(8) << buff->getEmptyTime() << " sekund\n";
		stat << "lacznie zaladowano danych: " << buff->getUploaded() << " bajtow\n";
		stat << "lacznie pobrano danych: " << buff->getDownloaded() << " bajtow\n";
		stat << "liczba bajtów odczytanych z pliku wejściowegoc: " << *in_bytes << "\n";
		stat << "liczba bajtów odczytanych z pliku wyjściowego: " << *out_bytes << "\n";


		delete buff;
	}

	cout << "statystyka zajetosci bufora znajduje sie w pliku: statistics.txt \n";



	plik_out.close();
	plik_hex.close();
	stat.close();
	delete in_bytes;
	return 0;
}




bool insertInSet(set<long long int> &s , long long int number)
{
	pair<set<long long int>::iterator , bool> result;

	result = s.insert(number);

	return result.second;
}
