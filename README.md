# DHCP
## Overview
The DHCP (Dynamic Host Configuration Protocol) library provides a dynamic IP address allocation mechanism similar to DHCP used in networking environments. It allows for the efficient allocation and freeing of IP addresses within a specified network range and subnet. The library offers functions for creating a DHCP structure, allocating IP addresses, freeing IP addresses, and counting the number of available free IP addresses.

## Library Functions
### DHCPCreate
```dhcp_t *DHCPCreate(const unsigned char network_address[BYTES_IN_IP], size_t num_bits_in_subnet);```

**Description:** This function creates a new DHCP data structure to manage IP address allocation within the specified network range and subnet.

**Parameters:**
* **network_address:** An unsigned char array representing the network address for which IP addresses will be allocated.
* **num_bits_in_subnet:** The number of bits in the subnet used for IP address allocation.

**Returns:** A pointer to the DHCP data structure on success, or NULL on failure.

### DHCPDestroy
```void DHCPDestroy(dhcp_t *dhcp);```

**Description:** This function destroys the DHCP data structure and frees any allocated resources.

**Parameters:**
* **dhcp:** A pointer to the DHCP data structure to be destroyed.

**Returns:** None.

### DHCPAllocateIP
```status_t DHCPAllocateIP(dhcp_t *dhcp, const unsigned char requested_ip[BYTES_IN_IP], unsigned char result_ip[BYTES_IN_IP]);```

**Description:** This function allocates an IP address from the DHCP data structure. If a specific IP is requested, it will be allocated if available; otherwise, the function allocates the next available IP.

**Parameters:**
* **dhcp:** A pointer to the DHCP data structure.
* **requested_ip:** An unsigned char array representing the requested IP address. If NULL, the requested IP is considered to be all zeroes.
* **result_ip:** An unsigned char array to store the allocated IP address.

**Returns:** SUCCESS on successful IP allocation or FAILURE if allocation fails.

### DHCPFreeIP
```status_t DHCPFreeIP(dhcp_t *dhcp, const unsigned char ip_to_free[BYTES_IN_IP]);```

**Description:** This function frees an allocated IP address, making it available for future allocation.

**Parameters:**
* **dhcp:** A pointer to the DHCP data structure.
* **ip_to_free:** An unsigned char array representing the IP address to be freed.
  
**Returns:** SUCCESS on successful IP freeing or IP_NOT_FOUND if the IP was not found in the DHCP data structure.

### DHCPCountFree
```size_t DHCPCountFree(const dhcp_t *dhcp);```

**Description:** This function returns the number of available free IP addresses in the DHCP data structure.

**Parameters:**
* **dhcp:** A pointer to the DHCP data structure.
  
**Returns:** The number of free IP addresses in the DHCP data structure.

## Usage
To use the DHCP library in your C project, follow these steps:

1. Include the dhcp.h header file in your project.
2. Link your project with the DHCP library during compilation.
3. Ensure that you provide valid network address and subnet information when creating the DHCP data structure. You can then use the DHCPAllocateIP and DHCPFreeIP functions to manage IP address allocation and deallocation.

### Example
```
#include <stdio.h>
#include "dhcp.h"

int main() {
    dhcp_t *dhcp = DHCPCreate((unsigned char[BYTES_IN_IP]){192, 168, 0, 0}, 24);
    unsigned char allocated_ip[BYTES_IN_IP] = {0};

    if (dhcp == NULL) {
        printf("Failed to create DHCP data structure.\n");
        return 1;
    }

    if (DHCPAllocateIP(dhcp, NULL, allocated_ip) != SUCCESS) {
        printf("Failed to allocate IP address.\n");
        return 1;
    }

    printf("Allocated IP: %d.%d.%d.%d\n", allocated_ip[0], allocated_ip[1], allocated_ip[2], allocated_ip[3]);

    DHCPFreeIP(dhcp, allocated_ip);
    DHCPDestroy(dhcp);

    return 0;
}
```

## Contributors
This project was developed by Lin Weintraub. For any questions or contributions, please contact the author.
