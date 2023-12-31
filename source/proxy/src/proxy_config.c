#include "proxy_config.h"

int random_number(size_t upperbound)
{
    return upperbound == 0 ? 0 : rand() % upperbound;
}

int calculate_lossiness(uint8_t drop_rate, uint8_t delay_rate, uint8_t corruption_rate)
{
    printf("delay: %u drop %u corruption: %u\n", delay_rate, drop_rate, corruption_rate);
    if (drop_rate > 0)
    {
        if (calculate_drop(drop_rate))
        {
            return DROP;
        }
    }

    if (delay_rate > 0)
    {
        if (calculate_delay(delay_rate))
        {
            return DELAY;
        }
    }

    if (corruption_rate > 0)
    {
        if (calculate_corruption(corruption_rate))
        {
            return CORRUPT;
        }
    }

    return SEND;
}

int calculate_drop(uint8_t percentage)
{
    int rand;
    rand = random_number(101);

    return rand > percentage ? FALSE : TRUE;
}

int calculate_delay(uint8_t percentage)
{
    int rand;
    rand = random_number(101);

    return rand > percentage ? FALSE : TRUE;
}

int calculate_corruption(uint8_t percentage)
{
    int rand;
    rand = random_number(101);

    return rand > percentage ? FALSE : TRUE;
}

int send_packet(int sockfd, packet *pt, struct sockaddr_storage *addr, FILE *fp)
{
    ssize_t result;

    result = sendto(sockfd, pt, sizeof(*pt), 0, (struct sockaddr *) addr,
                    size_of_address(addr));

    if (result < 0)
    {
        return -1;
    }

    write_stats_to_file(fp, pt);

    return 0;
}

int receive_packet(int sockfd, struct packet *pt, FILE *fp)
{
    struct sockaddr_storage     client_addr;
    socklen_t                   client_addr_len;
    struct packet               temp_pt;
    ssize_t                     result;

    client_addr_len = sizeof(client_addr);
    result = recvfrom(sockfd, &temp_pt, sizeof(temp_pt), 0, (struct sockaddr *) &client_addr, &client_addr_len);

    if (result == -1)
    {
        printf("Error: %s\n", strerror(errno));
        return -1;
    }

    *pt = temp_pt;
    write_stats_to_file(fp, pt);

    return 0;
}

void delay_packet(uint8_t delay_time)
{
    sleep(delay_time);
}

socklen_t size_of_address(struct sockaddr_storage *addr)
{
    return addr->ss_family == AF_INET ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);
}

void read_keyboard(uint8_t *client_drop, uint8_t *client_delay, uint8_t *server_drop, uint8_t *server_delay, uint8_t *corruption_rate) {
    int first_menu;
    int second_menu;
    int third_menu;
    char menu[200];
    char client_menu[100];

    snprintf(menu, sizeof (menu), "\nDynamic Proxy Lossiness Value:\n"
                                  "1. Client Losiness\n"
                                  "2. Server Losiness\n"
                                  "3. Data Corruption\n"
                                  "4. Exit\n"
                                  "Enter your Answer: ");

    snprintf(client_menu, sizeof (client_menu), "Client Drop and Delay rate:\n"
                                  "1. Drop Rate\n"
                                  "2. Delay Rate\n"
                                  "3. Back\n"
                                  "Enter your Answer: ");
    do {
        printf("%s", menu);
        first_menu = read_menu(4);
        switch (first_menu)
        {
            case 1:
                printf("%d\n", first_menu);
                do
                {
                    printf("%s", client_menu);
                    second_menu = read_menu(3);
                    switch (second_menu)
                    {
                        case 1:
                            printf("Enter Client's Drop Rate: ");
                            int drop =  read_menu(100);
                            if(drop == -1)
                            {
                                printf("Client's Drop Rate value should be between 0-100!\n");
                                break;
                            }
                            *client_drop = (uint8_t) drop;
                            break;
                        case 2:
                            printf("Enter Client's Delay Rate: ");
                            int delay =read_menu(100);
                            if(delay == -1)
                            {
                                printf("Client's Delay Rate value should be between 0-100!\n");
                                break;
                            }
                            *client_delay = (uint8_t) delay;
                            break;
                        case 3:
                            break;
                        case -1:
                            printf("It is not valid, try again\n");
                        default:
                            break;
                    }
                }
                while(second_menu != 3);
                break;
            case 2:
                do
                {
                    printf("Server Drop and Delay rate:\n");
                    printf("1. Drop Rate \n");
                    printf("2. Delay Rate \n");
                    printf("3. Back \n");
                    printf("Enter your Answer: ");
                    third_menu = read_menu(3);
                    switch (third_menu)
                    {
                        case 1:
                            printf("Enter Server's Drop Rate: ");
                            int drop =  read_menu(100);
                            if(drop == -1)
                            {
                                printf("Server's Drop Rate value should be between 0-100!\n");
                                break;
                            }
                            *server_drop = (uint8_t) drop;
                            break;

                        case 2:
                            printf("Enter Server's Delay Rate: ");
                            int delay =read_menu(100);
                            if(delay == -1)
                            {
                                printf("Server's Delay Rate value should be between 0-100!\n");
                                break;
                            }
                            *server_delay = (uint8_t) delay;
                            break;

                        case 3:
                            break;
                        case -1:
                            printf("It is not valid, try again\n\n");
                            break;
                        default:
                            break;
                    }
                }while(third_menu != 3);
                break;
            case 3:
                printf("Enter Data Corruption's Rate: ");
                int rate =  read_menu(100);
                if(rate == -1)
                {
                    printf("Data Corruption's Rate value should be between 0-100!\n");
                    break;
                }
                *corruption_rate = (uint8_t) rate;
                break;
            case 4:
                return;
            case -1:
                printf("It is not valid, try again\n");
            default:
                break;
        }
    } while (1);
}

int read_menu(int upperbound)
{
    char buf[128];
    errno = 0;
    fgets(buf, 128, stdin);

    char            *endptr;
    int temp = (int)strtol(buf, &endptr, 10);
    if (errno != 0)
    {
//        SET_ERROR(err, strerror(errno));

        return -1;
    }

    if(*endptr != '\n')
    {
        printf("end pointer: %c\n", *endptr);
        printf("%d ",temp);

        return -1;
    }
    if((int) temp > upperbound)
    {
        printf("\nlarger than upperbound!\n");
        return -1;
    }
    return (int) temp ;
}

int corrupt_data(char **data, size_t length)
{
    char *temp;

    temp = strdup(*data);

    for (size_t i = 0; i < length; i++)
    {
        int rbyte;
        int rbit;

        rbyte   = random_number(length);
        rbit    = random_number(8);

        temp[rbyte] ^= 1 << rbit;
    }

    *data = temp;

    return 0;
}

int write_stats_to_file(FILE *fp, const struct packet *pt)
{
    fprintf(fp, "%u,%u,%u,%u,%u,%s\n",
            pt -> hd.seq_number,
            pt -> hd.ack_number,
            pt -> hd.flags,
            pt -> hd.window_size,
            pt -> hd.checksum,
            pt -> data);
    fflush(fp);

    return 0;
}
