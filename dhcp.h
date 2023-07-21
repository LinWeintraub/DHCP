#ifndef __DHCP__
#define __DHCP__

#include <stddef.h> /* size_t */

#define BYTES_IN_IP 4

typedef struct dhcp dhcp_t;

typedef enum
{
    SUCCESS = 0,
    FAILURE = 1,
    IP_NOT_FOUND = 2
} status_t;


/*--------------------------------------------------------
Params: an unsigned char array representing a network_address for which the ip's
        will be allocated and the number of bits in the subnet.
Return: a pointer to the DHCP data structure created.
Complexity: O(log(n)) 
*/
dhcp_t *DHCPCreate(const unsigned char network_address[BYTES_IN_IP], 
                   size_t num_bits_in_subnet);


/*--------------------------------------------------------
Params: dhcp type pointer to dhcp object to be destroyed.
Return: None.
Complexity: O(n) 
*/
void DHCPDestroy(dhcp_t *dhcp);



/*--------------------------------------------------------
Params: A pointer to the DHCP structure. Requested ip and result ip represented 
        in unsigned char array. If the requested ip is NULL, the requested ip is 
        considered to be all zeroes. 
Return: SUCCESS on success and FAILURE if the allocation fails.
Complexity: O(long(n))
*/
status_t DHCPAllocateIP(dhcp_t *dhcp, 
                        const unsigned char requested_ip[BYTES_IN_IP], 
                        unsigned char result_ip[BYTES_IN_IP]);


/*--------------------------------------------------------
Params: A pointer to the DHCP structure and an unsigned char array representing 
        the ip to be freed. 
Return: SUCCESS on success and IP_NOT_FOUND if ip wasn't found.
Complexity: O(long(n))
*/
status_t DHCPFreeIP(dhcp_t *dhcp, const unsigned char ip_to_free[BYTES_IN_IP]);


/*--------------------------------------------------------
Params: A pointer to the DHCP structure.
Return: size_t type representing the number of free ip's in the DHCP data 
        structure.
Complexity: O(n)
*/
size_t DHCPCountFree(const dhcp_t *dhcp);

#endif /* __DHCP__ */
