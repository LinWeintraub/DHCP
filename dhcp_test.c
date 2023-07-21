/* ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ *
 ~ Author        Lin Weintraub                                   ~
 * Version       1.0.1                                           *
 ~ Date          6/12/22                                         ~
 * Description   DHCP                                            *
 * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ */
 
#include <stdio.h> /*printf*/

#include "dhcp.h"

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define CYAN "\033[1;36m"
#define WHITE "\033[0m"

#define TEST(name, actual, expected) \
    printf("%s: %s\n" , name, actual == expected ? GREEN"PASS"WHITE : RED"FAIL"WHITE)
#define TESTNOT(name, actual, expected) \
    printf("%s: %s\n" , name, actual != expected ? GREEN"PASS"WHITE : RED"FAIL"WHITE)

void TestCreateDestroy(void);
void TestAlloc(void);
void TestAlloc2(void);

int main(void)
{
    TestCreateDestroy();
	TestAlloc();
    TestAlloc2();

	return 0;
}

void TestCreateDestroy(void)
{
    dhcp_t *dhcp = NULL;
    const unsigned char network_address[] = {(unsigned char)192,(unsigned char)168,
                                            (unsigned char)1,(unsigned char)0};

    printf("----------Create & Destroy test:----------\n");
    dhcp = DHCPCreate(network_address, 24);

    TEST("Create test", (NULL != dhcp), 1);
    TEST("Dhcp Count empty should be 253", DHCPCountFree(dhcp), 253);

    DHCPDestroy(dhcp);
}

void TestAlloc(void)
{
    dhcp_t *dhcp = NULL;
    size_t i = 0;
    size_t j = 0;
    
    const unsigned char network_address[] = {(unsigned char)192,(unsigned char)168,
                                            (unsigned char)1,(unsigned char)0};
    const unsigned char requested_ip[] = {(unsigned char)192,(unsigned char)168,
                                                (unsigned char)1,(unsigned char)0};
    unsigned char result_ip[] = {(unsigned char)0,(unsigned char)0,
                                (unsigned char)0,(unsigned char)0};

    printf("----------Alloc test:----------\n");

    dhcp = DHCPCreate(network_address, 24);

    TEST("Dhcp Count empty should be 253", DHCPCountFree(dhcp), 253);
    for(j = 0; j < 15; j++)
    {
        DHCPAllocateIP(dhcp, requested_ip, result_ip);
        printf("Ask for 192.168.1.0: \n");
        
    
        printf("----------Alloced: ----------\n");
        for(i = 0; i < 4; i++)
        {
            printf("%d.", (unsigned int) result_ip[i]);
        }
        printf("\n------------------------------\n");

    }

    TEST("Dhcp count free, should be 238", DHCPCountFree(dhcp), 238);

    DHCPFreeIP(dhcp, network_address);

    TEST("After Free Ip should be 239", DHCPCountFree(dhcp), 239);

    DHCPDestroy(dhcp);

}

void TestAlloc2(void)
{
    dhcp_t *dhcp = NULL;

    size_t i = 0;
    size_t j = 0;
    
    const unsigned char network_address[] = {(unsigned char)1,(unsigned char)2,
                                            (unsigned char)0,(unsigned char)0};
    const unsigned char requested_ip[] = {(unsigned char)1,(unsigned char)2,
                                        (unsigned char)3,(unsigned char)4};
    unsigned char result_ip[] = {(unsigned char)0,(unsigned char)0,
                                (unsigned char)0,(unsigned char)0};

    dhcp = DHCPCreate(network_address, 10);

    TEST("Dhcp Count empty should be 4194301", DHCPCountFree(dhcp), 4194301);

    for(j = 0; j < 15; j++)
    {
        DHCPAllocateIP(dhcp, requested_ip, result_ip);
        printf("ask for 1.2.3.4: \n");
    
        printf("----------Alloced: ----------\n");
        for(i = 0; i < 4; i++)
        {
            printf("%d." ,(unsigned int) result_ip[i]);
        }

        printf("\n------------------------------\n");
    }

    TEST("Dhcp Count empty should be 4194286", DHCPCountFree(dhcp), 4194286);

    DHCPFreeIP(dhcp, network_address);

    TEST("Dhcp Count empty should be 4194286", DHCPCountFree(dhcp), 4194287);

    DHCPDestroy(dhcp);

}
