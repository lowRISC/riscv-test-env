/*
 * Simple DHCP Client
 * License : BSD
 * Author : Samuel Jacob (samueldotj@gmail.com)
 */
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "eth.h"

#define DHCP_BOOTREQUEST                    1
#define DHCP_BOOTREPLY                      2

#define DHCP_HARDWARE_TYPE_10_ETHERNET      1

#define MESSAGE_TYPE_PAD                    0
#define MESSAGE_TYPE_REQ_SUBNET_MASK        1
#define MESSAGE_TYPE_ROUTER                 3
#define MESSAGE_TYPE_DNS                    6
#define MESSAGE_TYPE_HOST_NAME              12
#define MESSAGE_TYPE_DOMAIN_NAME            15
#define MESSAGE_TYPE_NTP_SERVER             42
#define MESSAGE_TYPE_REQ_IP                 50
#define MESSAGE_TYPE_LEASE_TIME             51
#define MESSAGE_TYPE_DHCP                   53
#define MESSAGE_TYPE_SERVER_ID              54
#define MESSAGE_TYPE_PARAMETER_REQ_LIST     55
#define MESSAGE_TYPE_ERR                    56
#define MESSAGE_TYPE_CLIENT_ID              61
#define MESSAGE_TYPE_END                    255

#define DHCP_OPTION_DISCOVER                1
#define DHCP_OPTION_OFFER                   2
#define DHCP_OPTION_REQUEST                 3
#define DHCP_OPTION_DECLINE                 4
#define DHCP_OPTION_ACK                     5
#define DHCP_OPTION_NAK                     6
#define DHCP_OPTION_RELEASE                 7
#define DHCP_OPTION_INFORM                  8

typedef enum {
    VERBOSE_LEVEL_NONE,
    VERBOSE_LEVEL_ERROR,
    VERBOSE_LEVEL_INFO, 
    VERBOSE_LEVEL_DEBUG,
} verbose_level_t;

#define DHCP_MAGIC_COOKIE   0x63825363

const verbose_level_t program_verbose_level = VERBOSE_LEVEL_DEBUG;
u_int32_t ip;

static int
dhcp_request(u_int8_t *mac, u_int32_t req_ip, u_int32_t server_id);

/*
 * Print the Given ethernet packet in hexa format - Just for debugging
 */
static void
print_packet(const u_int8_t *data, int len)
{
    int i;

    for (i = 0; i < len; i++)
    {
        if (i % 0x10 == 0)
            printf("\n %04x :: ", i);
        printf("%02x ", data[i]);
    }
}

/*
 * Return checksum for the given data.
 * Copied from FreeBSD
 */
static unsigned short
in_cksum(unsigned short *addr, int len)
{
    register int sum = 0;
    u_short answer = 0;
    register u_short *w = addr;
    register int nleft = len;
    /*
     * Our algorithm is simple, using a 32 bit accumulator (sum), we add
     * sequential 16 bit words to it, and at the end, fold back all the
     * carry bits from the top 16 bits into the lower 16 bits.
     */
    while (nleft > 1)
    {
        sum += *w++;
        nleft -= 2;
    }
    /* mop up an odd byte, if necessary */
    if (nleft == 1)
    {
        *(u_char *)(&answer) = *(u_char *) w;
        sum += answer;
    }
    /* add back carry outs from top 16 bits to low 16 bits */
    sum = (sum >> 16) + (sum & 0xffff);     /* add hi 16 to low 16 */
    sum += (sum >> 16);             /* add carry */
    answer = ~sum;              /* truncate to 16 bits */
    return (answer);
}

/*
 * This function will be called for any incoming DHCP responses
 */
void dhcp_input(dhcp_t *dhcp, u_int8_t *mac, int *offcount, int *ackcount)
{
  int len = 0;
  char err[256];
  char domain[256];
  char hostname[256];
  u_int8_t code;
  u_int16_t   flags;
  uint32_t lease = 0;
  u_int8_t option = 0;
  uip_ipaddr_t req_ip, server_id, netmask, router, dns_id, ntp_id;
  *domain = 0;
  *hostname = 0;
  *err = 0;
  if ((uint64_t)dhcp < 0x40000000 || (uint64_t)dhcp >= 0x88000000)
    {
      printf("dhcp internal error, %x\n", dhcp);
      for(;;)
        ;
    }
  if (dhcp->opcode != 2)
    return;
  do {
    u_int8_t *opt_field = &dhcp->bp_options[len];
    code  = opt_field[0];
    u_int8_t field_len = opt_field[1];
    u_int8_t *data = &opt_field[2];
    len += field_len + (sizeof(u_int8_t) * 2);
    switch(code)
      {
      case MESSAGE_TYPE_DNS:
        memcpy(&dns_id, data, sizeof(dns_id));
        break;
      case MESSAGE_TYPE_HOST_NAME:
        memcpy(hostname, data, field_len);
        hostname[field_len] = 0;
        break;
      case MESSAGE_TYPE_NTP_SERVER:
        memcpy(&ntp_id, data, sizeof(ntp_id));
        break;
      case MESSAGE_TYPE_SERVER_ID:
        memcpy(&server_id, data, sizeof(server_id));
        break;
      case MESSAGE_TYPE_LEASE_TIME:
        memcpy(&lease, data, sizeof(lease));
        lease = ntohl(lease);
        break;
      case MESSAGE_TYPE_REQ_SUBNET_MASK:
        memcpy(&netmask, data, sizeof(netmask));
        break;
      case MESSAGE_TYPE_ROUTER:
        memcpy(&router, data, sizeof(router));
        break;
      case MESSAGE_TYPE_DOMAIN_NAME:
        memcpy(domain, data, field_len);
        domain[field_len] = 0;
        break;
      case MESSAGE_TYPE_DHCP:
        memcpy(&option, data, sizeof(option));
        break;
      case MESSAGE_TYPE_ERR:
        memcpy(err, data, field_len);
        err[field_len] = 0;
        break;
      case MESSAGE_TYPE_END:
        switch(option)
          {
          case DHCP_OPTION_OFFER:
            if (!*offcount) // Only accept the first offer
              {
                u_int32_t req_ip, server_id;                
                ++*offcount;
                /* Get the IP address given by the server */
                memcpy(&req_ip, &(dhcp->yiaddr), sizeof(u_int32_t));
                memcpy(&server_id, &(dhcp->siaddr), sizeof(u_int32_t));
                dhcp_request(mac, req_ip, server_id);
              }
            break;
          case DHCP_OPTION_ACK:
            if (!*ackcount)
              {
                ++*ackcount;
                printf("DHCP ACK\n");            
                /* Get the IP address given by the server */
                memcpy(&req_ip, &(dhcp->yiaddr), sizeof(uint32_t));
                memcpy(&server_id, &(dhcp->siaddr), sizeof(uint32_t));
                printf("DHCP Client IP Address:  %d.%d.%d.%d\n", uip_ipaddr_to_quad(&req_ip));
                uip_sethostaddr(&req_ip);
                printf("Server IP Address:  %d.%d.%d.%d\n", uip_ipaddr_to_quad(&server_id));
                printf("Router address:  %d.%d.%d.%d\n", uip_ipaddr_to_quad(&router));
                printf("Net mask address:  %d.%d.%d.%d\n", uip_ipaddr_to_quad(&netmask));
                uip_setnetmask(&netmask);
                printf("Lease time = %d\n", lease);
                printf("domain = \"%s\"\n", domain);
                printf("server = \"%s\"\n", hostname);
              }
            else
              printf("ACK SKIPPED\n");
            break;
          case DHCP_OPTION_NAK:
            if (!*ackcount)
              {
                ++*ackcount;
                printf("DHCP NAK\n");            
                printf("Requested address refused\n");
                printf("Error %s\n", err);
              }
            break;
          default:
            printf("unhandled option %d\n", option);
          }
        break;
      default:
        printf("Unhandled DHCP opcode %d\n", code);
      }
  } while (code != MESSAGE_TYPE_END);
  //  printf("DHCP process exited\n");
}

int my_inject(const void *buf, size_t siz)
{
  lite_queue(buf, siz);
  return siz;
}

void my_perror(char *msg)
{
  printf("perror(%s)\n", msg);
}

/*
 * Ethernet output handler - Fills appropriate bytes in ethernet header
 */
static void
ether_output(u_char *frame, const u_int8_t *mac, int len, const u_int8_t *destmac)
{
    int result;
    struct ether_header *eframe = (struct ether_header *)frame;

    memcpy(eframe->ether_shost, mac, ETHER_ADDR_LEN);
    memcpy(eframe->ether_dhost, destmac,  ETHER_ADDR_LEN);
    eframe->ether_type = __htons(ETHERTYPE_IP);

    len = len + sizeof(struct ether_header);

    /* Send the packet on wire */
    result = my_inject(frame, len);
}

/*
 * IP Output handler - Fills appropriate bytes in IP header
 */
static void
ip_output(struct ip *ip_header, int *len, uint32_t srcaddr, uint32_t dstaddr)
{
  u_short nlen, nid;

    *len += sizeof(struct ip);

    ip_header->ip_hl = 5;
    ip_header->ip_v = IPVERSION;
    ip_header->ip_tos = 0x10;
    nlen = __htons(*len);
    nid = __htons(0xffff);
    memcpy(&(ip_header->ip_len), &nlen, sizeof(u_short));
    memcpy(&(ip_header->ip_id), &nid, sizeof(u_short));
    ip_header->ip_off = 0;
    ip_header->ip_ttl = 16;
    ip_header->ip_p = IPPROTO_UDP;
    ip_header->ip_sum = 0;
    memcpy(&(ip_header->ip_src.s_addr), &srcaddr, sizeof(uint32_t));
    memcpy(&(ip_header->ip_dst.s_addr), &dstaddr, sizeof(uint32_t));

    ip_header->ip_sum = in_cksum((unsigned short *) ip_header, sizeof(struct ip));
}

/*
 * UDP output - Fills appropriate bytes in UDP header
 */
static void
udp_output(struct udphdr *udp_header, int *len, uint16_t client, uint16_t server)
{
    if (*len & 1)
        *len += 1;
    *len += sizeof(struct udphdr);

    udp_header->uh_sport = __htons(client);
    udp_header->uh_dport = __htons(server);
    udp_header->uh_ulen = __htons(*len);
    udp_header->uh_sum = 0;
}

/*
 * DHCP output - Just fills DHCP_BOOTREQUEST
 */
static void
dhcp_output(dhcp_t *dhcp, u_int8_t *mac, int *len)
{
    *len += sizeof(dhcp_t);
    memset(dhcp, 0, sizeof(dhcp_t));

    dhcp->opcode = DHCP_BOOTREQUEST;
    dhcp->htype = DHCP_HARDWARE_TYPE_10_ETHERNET;
    dhcp->hlen = 6;
    memcpy(dhcp->chaddr, mac, DHCP_CHADDR_LEN);

    dhcp->magic_cookie = __htonl(DHCP_MAGIC_COOKIE);
}

/*
 * Adds DHCP option to the bytestream
 */
static int
fill_dhcp_option(u_int8_t *packet, u_int8_t code, u_int8_t *data, u_int8_t len)
{
    packet[0] = code;
    packet[1] = len;
    memcpy(&packet[2], data, len);

    return len + (sizeof(u_int8_t) * 2);
}

/*
 * Fill DHCP options
 */
static int
fill_dhcp_discovery_options(dhcp_t *dhcp)
{
    int len = 0;
    u_int32_t req_ip;
    u_int8_t parameter_req_list[] = {MESSAGE_TYPE_REQ_SUBNET_MASK, MESSAGE_TYPE_ROUTER, MESSAGE_TYPE_DNS, MESSAGE_TYPE_DOMAIN_NAME};
    u_int8_t option;

    option = DHCP_OPTION_DISCOVER;
    len += fill_dhcp_option(&dhcp->bp_options[len], MESSAGE_TYPE_DHCP, &option, sizeof(option));
#if 0
    req_ip = __htonl(0xc0a8010a);
    len += fill_dhcp_option(&dhcp->bp_options[len], MESSAGE_TYPE_REQ_IP, (u_int8_t *)&req_ip, sizeof(req_ip));
#endif    
    len += fill_dhcp_option(&dhcp->bp_options[len], MESSAGE_TYPE_PARAMETER_REQ_LIST, (u_int8_t *)&parameter_req_list, sizeof(parameter_req_list));
    option = 0;
    len += fill_dhcp_option(&dhcp->bp_options[len], MESSAGE_TYPE_END, &option, sizeof(option));

    return len;
}

/*
 * Send DHCP DISCOVERY packet
 */
static int
dhcp_discovery(u_int8_t *mac)
{
    int len = 0;
    u_char packet[4096];
    struct udphdr *udp_header;
    struct ip *ip_header;
    dhcp_t *dhcp;
    u_char broadcast[ETHER_ADDR_LEN];

    printf("Sending DHCP_DISCOVERY\n");

    memset(broadcast, -1, ETHER_ADDR_LEN);
    ip_header = (struct ip *)(packet + sizeof(struct ether_header));
    udp_header = (struct udphdr *)(((char *)ip_header) + sizeof(struct ip));
    dhcp = (dhcp_t *)(((char *)udp_header) + sizeof(struct udphdr));

    len = fill_dhcp_discovery_options(dhcp);
    dhcp_output(dhcp, mac, &len);
    udp_output(udp_header, &len, DHCP_CLIENT_PORT, DHCP_SERVER_PORT);
    ip_output(ip_header, &len, 0, 0xFFFFFFFF);
    ether_output(packet, mac, len, broadcast);

    return 0;
}

/*
 * Fill DHCP options
 */
static int
fill_dhcp_request_options(dhcp_t *dhcp, u_int32_t req_ip, u_int32_t server_id)
{
    int len = 0;
    u_int8_t parameter_req_list[] = {MESSAGE_TYPE_REQ_IP, MESSAGE_TYPE_SERVER_ID};
    u_int8_t option;

    option = DHCP_OPTION_REQUEST;
    len += fill_dhcp_option(&dhcp->bp_options[len], MESSAGE_TYPE_DHCP, &option, sizeof(option));
    len += fill_dhcp_option(&dhcp->bp_options[len], MESSAGE_TYPE_REQ_IP, (u_int8_t *)&req_ip, sizeof(req_ip));
    len += fill_dhcp_option(&dhcp->bp_options[len], MESSAGE_TYPE_SERVER_ID, (u_int8_t *)&server_id, sizeof(server_id));
    option = 0;
    len += fill_dhcp_option(&dhcp->bp_options[len], MESSAGE_TYPE_END, &option, sizeof(option));

    return len;
}

/*
 * Send DHCP REQUEST packet
 */
static int
dhcp_request(u_int8_t *mac, u_int32_t req_ip, u_int32_t server_id)
{
    int len = 0;
    u_char packet[4096];
    struct udphdr *udp_header;
    struct ip *ip_header;
    dhcp_t *dhcp;
    u_char broadcast[ETHER_ADDR_LEN];
    
    printf("Sending DHCP_REQUEST\n");

    memset(broadcast, -1, ETHER_ADDR_LEN);
    ip_header = (struct ip *)(packet + sizeof(struct ether_header));
    udp_header = (struct udphdr *)(((char *)ip_header) + sizeof(struct ip));
    dhcp = (dhcp_t *)(((char *)udp_header) + sizeof(struct udphdr));

    len = fill_dhcp_request_options(dhcp, req_ip, server_id);
    dhcp_output(dhcp, mac, &len);
    udp_output(udp_header, &len, DHCP_CLIENT_PORT, DHCP_SERVER_PORT);
    ip_output(ip_header, &len, 0, 0xFFFFFFFF);
    ether_output(packet, mac, len, broadcast);

    return 0;
}

/*
 * Send Generic UDP packet
 */
int udp_send(const u_int8_t *mac, void *msg, int payload_size, uint16_t client, uint16_t server, uint32_t srcaddr, uint32_t dstaddr, const u_int8_t *destmac)
{
    int len = 0;
    u_char packet[2048];
    struct udphdr *udp_header;
    struct ip *ip_header;
    u_char *payload;

    ip_header = (struct ip *)(packet + sizeof(struct ether_header));
    udp_header = (struct udphdr *)(((char *)ip_header) + sizeof(struct ip));
    payload = (u_char *)(((char *)udp_header) + sizeof(struct udphdr));
    memcpy(payload, msg, payload_size);
    len = payload_size;
    udp_output(udp_header, &len, client, server);
    ip_output(ip_header, &len, srcaddr, dstaddr);
    ether_output(packet, mac, len, destmac);

    return 0;
}

int dhcp_main(u_int8_t mac[6])
{
    int result;
    char errbuf[PCAP_ERRBUF_SIZE];
    char *dev = "eth0";

    printf("%s MAC : %02X:%02X:%02X:%02X:%02X:%02X\n",
          dev, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    
    /* Send DHCP DISCOVERY packet */
    result = dhcp_discovery(mac);
    if (result)
    {
        printf("Couldn't send DHCP DISCOVERY on device %s: %s\n", dev, errbuf);
    }

    else
      {
        ip = 0;
        printf("Waiting for DHCP_OFFER\n");
      }
    return result;
}
