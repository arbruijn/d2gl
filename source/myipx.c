#include <string.h>
#include <stdlib.h>
#include "pstypes.h"
#include "ipx.h"

void net_init(ubyte *node) { }
void net_send(ubyte *node, ubyte *data, int len) { }
void net_send_all(ubyte *data, int len) { }
int net_receive(ubyte *data, int max_len) { return 0; }

int ipx_init( int socket_number, int show_address ) { return 0; }
int ipx_change_default_socket( ushort socket_number ) { return 0; }
ubyte ipx_net[4], ipx_node[6], ipx_has_addr;
static void ipx_init_addr() {
	pssrand(timer_get_fixed_seconds());
	for (int i = 0; i < 6; i++)
		ipx_node[i] = psrand() / (PSRAND_MAX >> 8);
	//printf("ipx address %02x:%02x:%02x:%02x:%02x:%02x\n", ipx_node[0], ipx_node[1], ipx_node[2], ipx_node[3], ipx_node[4], ipx_node[5]);
	ipx_has_addr = 1;
	net_init(ipx_node);
}
ubyte * ipx_get_my_local_address() { if (!ipx_has_addr) ipx_init_addr(); return ipx_node; }
ubyte * ipx_get_my_server_address() { if (!ipx_has_addr) ipx_init_addr(); return ipx_net; }
void ipx_get_local_target( ubyte * server, ubyte * node, ubyte * local_target ) { }
int ipx_get_packet_data( ubyte * data ) { return net_receive(data, IPX_MAX_DATA_SIZE); }
void ipx_send_broadcast_packet_data( ubyte * data, int datasize ) { net_send_all(data, datasize); }
void ipx_send_packet_data( ubyte * data, int datasize, ubyte *network, ubyte *address, ubyte *immediate_address ) { net_send(address, data, datasize); }
void ipx_send_internetwork_packet_data( ubyte * data, int datasize, ubyte * server, ubyte *address ) { net_send(address, data, datasize); }
void ipx_read_user_file(char * filename) {}
void ipx_read_network_file(char * filename) {}
