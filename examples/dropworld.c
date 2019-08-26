    #include <linux/bpf.h>

int prog(struct xdp_md *ctx){
    return XDP_DROP;
}