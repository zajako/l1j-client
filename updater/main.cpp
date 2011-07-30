#include <stdio.h>

#include "config.h"
#include "connection.h"
#include "globals.h"
#include "lindes.h"
#include "music.h"
#include "pack.h"
#include "packet.h"
#include "table.h"
#include "unsorted.h"

#define TRANSFER_AMOUNT 0x400

unsigned long checksum = 0xdeadbeef;
pack *textpack;
pack *tilepack;
pack **spritepack;
const int num_sprite_pack = 17;
table bad_words;
table strings;
table teleport;
table un_transaction;
table important_items;
table pet;
table solvent;
table todays_tip;

int pack_resources()
{
	return 0;
}

int init_packs()
{
	textpack = new pack("Text", 1);
	return 0;
	tilepack = new pack("Tile", 0);
	spritepack = new pack*[num_sprite_pack];
	spritepack[0] = new pack("Sprite", 0);
	for (int i = 0; i < (num_sprite_pack-1); i++)
	{
		char name[10];
		sprintf(name, "Sprite%02d", i);
		spritepack[i+1] = new pack(name, 0);
	}
	return 0;
}

int init_strings()
{
	int acp = -1;
	if (acp == -1)
	{
		printf("STUB GetACP()\n");
		//acp = GetACP();
	}
	
	//list of filtered chat words
	bad_words.load_local("obscene");
	bad_words.sort();
	
	//list of tips for new players?
	todays_tip.load_local("todaystip");
	printf("STUB Load MercenaryIconData\n");
	printf("STUB Load MagicDollData\n");
	
	//list of items
	solvent.load_local("solvent");
	
	//list of pet types
	pet.load_local("ntexpet");
	
	//no important items?
	important_items.load("itemimportant.tbl");
	
	printf("STUB Load BaseStatus\n");
	
	//unknown
	teleport.load("telbook.tbl");
	
	//unknown
	un_transaction.load("untransaction.tbl");
	
//	if (battleServer != 0)
//	{
//		table notices;
//		notices.load("notice_bs.tbl");
//		notices.print();
//	}

	strings.load_local("string");
	
	return 0;
}

int get_updates(connection* server)
{
	unsigned long temp;
	long sign_temp;
	int status;	//> 0 means update occurred
	status = 0;
	printf("STUB Get update magic number\n");
	try
	{
		if (server->connection_ok() == 1)
		{
			server->snd_var(&checksum, 4);
			server->rcv_var(&sign_temp, 4);
			if (sign_temp < 0)
			{
				temp = -sign_temp;
				printf("Protocol : %lx, ServerId : %lx\n", temp & 0xFFFF, temp>>16);
				server->rcv_var(&sign_temp, 4);
			}
			if (sign_temp > 0)
			{	//receive files
				int num_files = sign_temp;
				printf("Receiving %d files\n", num_files);
				status = num_files;
				unsigned char name_length;
				unsigned long file_length;
				unsigned char* file_buffer;
				char *filename;
				for (int i = 0; i < num_files; i++)
				{
					server->rcv(&name_length, 1);
					filename = new char[name_length];
					server->rcv(filename, name_length);
					filename[name_length] = 0;
					server->rcv_var(&file_length, 4);
					file_buffer = new unsigned char[file_length];
					printf("Downloading %s, %ld bytes ...", filename, file_length);
					//do stuff so the file can be saved
					char *dump_name;
					dump_name = new char[name_length + 6];
					sprintf(dump_name, "%s.gz", filename);
//					FILE *filedump = fopen(dump_name, "wb");
					while (file_length > 0)
					{
						if (file_length > TRANSFER_AMOUNT)
						{
							server->rcv(file_buffer, TRANSFER_AMOUNT);
//							fwrite(file_buffer, 1, TRANSFER_AMOUNT, filedump);
							file_length -= TRANSFER_AMOUNT;
						}
						else
						{
							server->rcv(file_buffer, file_length);
//							fwrite(file_buffer, 1, file_length, filedump);
							file_length = 0;
						}
					}
//					fclose(filedump);
					printf(" done\n");
					printf("STUB Update magic number\n");
					server->rcv_var(&sign_temp, 4);
					delete [] dump_name;
					delete [] filename;
					delete [] file_buffer;
				}
			}
			long num_servers;
			unsigned short* num_users;
			server->rcv_var(&num_servers, 4);
			num_users = new unsigned short[num_servers];
			for (int j = 0; j < num_servers; j++)
				server->rcv_var(&num_users[j], 2);
		}
	}
	catch(int e)
	{
		printf("An error occurred %d\n", e);
		status = -1;
	}
	return status;
}

int main (int argc, char **argv)
{
	config *main_config;
	connection *server;

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
	{
		printf("WSAStartup failed!\n");
//		perror("WSAStartup failed!\n");
		return 1;
	}
	
	main_config = new config("Lineage.ini");
	if (!main_config->config_ok())
	{
		printf("ERROR Loading configuration file.\n");
		delete main_config;
		WSACleanup();
		return 1;
	}
	else
	{
//		printf("Configuration loaded successfully.\n");
	}

#if 0
	pack *testme;
	testme = new pack("Text", 1);
	testme->detect_dupes();
	int file = 11;
	printf("\tFile number %d:\n\t%s\n", file, testme->load_file(11));
	delete testme;
	testme = new pack("Tile", 0);
	testme->detect_dupes();
	delete testme;
	WSACleanup();
	return 0;
#endif
	DesKeyInit("~!@#%^$<");
	init_packs();

	server = new connection(main_config);
	if (get_updates(server) > 0)
	{
		printf("STUB Packing resources\n");
	}
	
	//begin game portion of client
	if (server->change() == 1)
	{
//		printf("Connected to game server\n");
		packet bob(server);
	}
	else
	{
		printf("Failed to connect to game server\n");
	}
	
	music game_music;
	if (game_music.init() != 0)
	{
		//music failed
		printf("STUB InitSound()\n");
	}
	
	init_codepage(0);
	init_math_tables();
	printf("STUB Load player config\n");
	printf("STUB Initialize screenshots\n");
	printf("STUB Initialize emblem cache\n");
	printf("STUB Initialize GUI\n");

	init_strings();
	
	printf("Exiting client now\n");
	delete server;	
	delete main_config;
	WSACleanup();
	return 0;
}
