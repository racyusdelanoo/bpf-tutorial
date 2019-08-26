#include <linux/if_ether.h>
#include <netinet/ip.h>
#include "ebpf_switch.h"

struct bpf_map_def SEC("maps") counter = {
        .type = BPF_MAP_TYPE_FOO,
};


uint64_t prog(struct packet *pkt)
{
    uint32_t *num_packets;
    //bpf_debug2(&(pkt->eth.h_proto), 1);

    if ((pkt->eth.h_proto&0x8) == 0x8){ //ipv4 header
	bpf_map_update_elem(&counter, &(pkt->eth.h_proto), 0, 0);
        
	bpf_map_lookup_elem(&counter, 0, &num_packets);
        bpf_notify(0, num_packets, sizeof(uint32_t));
    }

    return FLOOD;
}
char _license[] SEC("license") = "GPL";
