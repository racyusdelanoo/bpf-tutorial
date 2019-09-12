#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/in.h>
#include "bpf_endian.h"

int isTCP( struct xdp_md *ctx ) {
    void *data_end = (void *)(long) ctx->data_end;
    void *data_begin = (void *)(long) ctx->data;
    struct ethhdr* eth = data_begin;
    
    // Check packet's size
    if(eth + 1 > data_end)
      return XDP_PASS;
    
    // Check if Ethernet frame has IPv4 packet
    if (eth->h_proto == bpf_htons( ETH_P_IP )) { 
        struct iphdr *ipv4 = (struct iphdr *)( ((void*)eth) + ETH_HLEN );

        if(ipv4 + 1 > data_end)
          return XDP_PASS;

        // Check if IPv4 packet contains a TCP segment
        if (ipv4->protocol == IPPROTO_TCP) 
          return XDP_PASS;
    } 
    return XDP_DROP;
}