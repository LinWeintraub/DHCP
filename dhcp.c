/* ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ *
 ~ Author        Lin Weintraub                                   ~
 * Version       1.0.1                                           *
 ~ Date          6/12/22                                         ~
 * Description   DHCP                                            *
 * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ */
 
#include <stdlib.h> /* malloc */
#include <string.h> /* memove */
#include <assert.h> /* assert */

#include "dhcp.h"
#include "trie.h"

#define BITS_IN_IP 32
#define BITS_IN_BYTE 8

struct dhcp
{
    trie_t *trie;
    size_t num_bits_in_subnet;
	unsigned char network_address[BYTES_IN_IP];
};

static unsigned int BitCmp(dhcp_t *dhcp, const unsigned char ip[BYTES_IN_IP]);

/*---------------------------------API-Functions------------------------------*/
dhcp_t *DHCPCreate(const unsigned char network_address[BYTES_IN_IP], 
                   size_t num_bits_in_subnet)
{
    dhcp_t *dhcp = malloc(sizeof(dhcp_t)); 
    unsigned char result_ip[BYTES_IN_IP] = {0};
    unsigned char network_addr[BYTES_IN_IP] = {0,0,0,0};
    unsigned char server_addr[BYTES_IN_IP] = {255,255,255,254};
    unsigned char broadcast_addr[BYTES_IN_IP] = {255,255,255,255};

    assert(network_address);
    assert(30 >= num_bits_in_subnet);

    if (NULL == dhcp)
    {
        return NULL;
    }

    dhcp->num_bits_in_subnet = num_bits_in_subnet;

    dhcp->trie = TrieCreate(BITS_IN_IP - num_bits_in_subnet);
    if (NULL == dhcp->trie)
    {
        free(dhcp);
        return NULL;
    }

    if (FAILURE == DHCPAllocateIP(dhcp, network_addr, result_ip))
    {
        DHCPDestroy(dhcp);
        return NULL;
    }

    if (FAILURE == DHCPAllocateIP(dhcp, server_addr, result_ip))
    {
        DHCPDestroy(dhcp);
        return NULL;
    }

    if (FAILURE == DHCPAllocateIP(dhcp, broadcast_addr, result_ip))
    {
        DHCPDestroy(dhcp);
        return NULL;
    }

   *(unsigned int *)dhcp->network_address = *(unsigned int *)network_address;

    return dhcp;
}
/*----------------------------------------------------------------------------*/
void DHCPDestroy(dhcp_t *dhcp)
{
    assert(dhcp);

    TrieDestroy(dhcp->trie);

    free(dhcp);
}
/*----------------------------------------------------------------------------*/
status_t DHCPAllocateIP(dhcp_t *dhcp, 
                        const unsigned char requested_ip[BYTES_IN_IP], 
                        unsigned char result_ip[BYTES_IN_IP])
{
    size_t i = 0;
    unsigned int request = 0;
    unsigned int network = 0;
    unsigned int subnet_mask = 0xFFFFFFFF; /*F = 1*/
    unsigned int host_mask = 0xFFFFFFFF; /*F = 1*/
    trie_status_t trie_status = TRIE_FAILURE;
    status_t dhcp_status = FAILURE;
    unsigned char *result_ptr = NULL;

    assert(dhcp);
    assert(requested_ip);

    request = requested_ip[0];
    network = dhcp->network_address[0];

    for (i = 1; i < BYTES_IN_IP; i++)
    {
        network = network << BITS_IN_BYTE;
        network = network | dhcp->network_address[i];

        request = request << BITS_IN_BYTE;
        request = request | requested_ip[i];
    }

    host_mask = host_mask >> dhcp->num_bits_in_subnet; /*0000000000000000000000111111111*/
    subnet_mask = ~host_mask; /*1111111110000000000000000*/

    while (0 != TrieCountFree(dhcp->trie))
    {
        trie_status = TrieInsert(dhcp->trie, request);
        if (TRIE_FAILURE == trie_status)
        {
            ++request;
        }

        else
        {
            dhcp_status = SUCCESS;            
            request =  (request & host_mask) | (network & subnet_mask);
            result_ptr = (unsigned char *)&request;

            for (i = BYTES_IN_IP - 1; 0 != i; --i)
            {
                result_ip[i] = *result_ptr;
                ++result_ptr;
            }

            result_ip[i] = *result_ptr;

            break;
        }
    }

    return dhcp_status;
}
/*----------------------------------------------------------------------------*/
status_t DHCPFreeIP(dhcp_t *dhcp, const unsigned char ip_to_free[BYTES_IN_IP])
{
    size_t remove_ip = 0;

    assert(dhcp);
    assert(ip_to_free);

    remove_ip = *(unsigned char *)ip_to_free;
    remove_ip = remove_ip >> dhcp->num_bits_in_subnet;

    if (0 != BitCmp(dhcp, ip_to_free))
    {
        return IP_NOT_FOUND;
    }

    if (TRIE_FAILURE == TrieRemove(dhcp->trie, remove_ip))
    {
        return IP_NOT_FOUND;
    }

    return SUCCESS;
}
/*----------------------------------------------------------------------------*/
size_t DHCPCountFree(const dhcp_t *dhcp)
{
    assert(dhcp);

    return(TrieCountFree(dhcp->trie));
}

/*------------------------------Static-Functions------------------------------*/
static unsigned int BitCmp(dhcp_t *dhcp, const unsigned char ip[BYTES_IN_IP])
{
    unsigned int network_ip = 0;
    unsigned int network_dhcp = 0;
    unsigned int subnet_mask = 0xFFFFFFFF; /*F = 1*/
    unsigned int host_mask = 0xFFFFFFFF; /*F = 1*/
    int i = 0;

    assert(dhcp);
    assert(ip);

    network_ip = ip[0];
    network_dhcp = dhcp->network_address[0];

    for (i = 1; i < BYTES_IN_IP; i++)
    {
        network_dhcp = network_dhcp << BITS_IN_BYTE;
        network_dhcp = network_dhcp | dhcp->network_address[i];

        network_ip = network_ip << BITS_IN_BYTE;
        network_ip = network_ip | ip[i];
    }

    host_mask = host_mask >> dhcp->num_bits_in_subnet; /*00001111*/
    subnet_mask = ~host_mask; /*11110000*/

    network_ip = (network_ip & subnet_mask);
    network_dhcp = (network_dhcp & subnet_mask);
    
    return (network_ip - network_dhcp);
}
