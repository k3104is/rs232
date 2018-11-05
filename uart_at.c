// #define UNICODE
/**************************************************

file: demo_tx.c
purpose: simple demo that transmits characters to
the serial port and print them on the screen,
exit the program by pressing Ctrl-C

compile with the command: gcc -std=c99 uart_at.c rs232.c -Wall -Wextra -o2 -o uart_at

**************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "rs232.h"
#include "uart_at.h"

#ifdef _WIN32
#define sleep_uart()    Sleep(1000)
#else
#define sleep_uart()    usleep(1000000)  /* sleep for 1 Second */
#endif
// #define strstr         (NULL != strstr((char *)u1_buf, s1_res) 

unsigned char init(void);
void discriptionSelect(void);
void tx(void);
void rx(void);
void txScanf(char *s1_str);
void hex(char *s1_str, unsigned char n);
void MakeTxCommand(char *s1_str, char *s1_ope);
void txSelect(char *s1_str);
void discriptionScan(void);
void discriptionMode(void);
unsigned char txMode(unsigned char u1_mode, char *s1_str);
unsigned char vdChkSuffix(char *u1_str);

static const S_SETTING c_s_t_setting =
// const static S_SETTING c_s_t_setting =
{
    15,             /* ポート番号 */
    115200,         /* ボーレート(bps) */
    "8N1"           /* キャラクタ設定 */
};
#define _DEF_SIERRA_
#ifndef _DEF_SIERRA_
static const S_AT c_s_t_at[] =
{
    {"AT"                                               ,"Tests AT startup."                                        },        /* ATコマンド */
    {"AT+GMR"                                           ,"Checks version information."                              },        /* ATコマンド */
    {"AT+CWLAP"                                         ,"Lists available APs."                                     },        /* ATコマンド */
    {"AT+CWJAP=\"test_ap\",\"akasatana\""               ,"Connects to an AP(test_ap)."                              },        /* ATコマンド */
    {"AT+CWJAP=\"SPWH_H32_E57283\",\"t90jnhaet0aagbn\"" ,"Connects to an AP(SPWH_H32_E57283)."                      },        /* ATコマンド */
    {"AT+CWQAP"                                         ,"Disconnects from an AP"                                   },        /* ATコマンド */
};
#else
static const S_AT c_s_t_at[] =
{
    {"AT"                                               ,"Tests AT startup."                                        },        /* ATコマンド */
    {"AT&K3"                                            ,"Hardware flow control activation"                         },        /* ATコマンド */
    {"AT+KCNFG=1,\"GRRS\",\"kddiint.lpwa.kddi.com\",\"user\",\"guest\",\"0.0.0.0\",\"0.0.0.0\",\"0.0.0.0\""
                                                        ,"Set GPRS parameters (APN, login, password)"               },        /* ATコマンド */
    {"AT+KTCPCFG=1,0,\"xxx,xxx,xxx,xxx\",00"            ,"Set IP address and port number Returns session ID"        },        /* ATコマンド */
    {"AT+KTCPSTART=1"                                   ,"Initiate the connection, use session 1"                   },        /* ATコマンド */
    {"+++"                                              ,"Use +++ to enter in command mode"                         },        /* ATコマンド */
    {"AT+KTCPCLOSE=1,1"                                 ,"Use KTCPCLOSE to close the session"                       },        /* ATコマンド */
    {"AT+KTCPDEL=1"                                     ,"Delete the configured session"                            },        /* ATコマンド */
};
#endif
static const unsigned char c_s_u1_numof_at = sizeof(c_s_t_at) / sizeof(S_AT);

unsigned char init(void)
{
    if(RS232_OpenComport(   c_s_t_setting.u1_cport_nr,
                            c_s_t_setting.u4_bdrate,
                            c_s_t_setting.s1_mode))
    {
        printf("Can not open comport\n");
        return eNG;
    }
    return eOK;
}

void tx(void)
{
    unsigned char i, u1_mode;
    char s1_str[100] = "", s1_scan[10];

    while(1)
    {
        discriptionMode();
        scanf("%s,", s1_scan);
        u1_mode = (unsigned char)atoi(s1_scan);
        printf("mode : %d", u1_mode);
        if(txMode(u1_mode, s1_str)) break;
    }
    for(i = 0; s1_str[i] != '\0'; i++);
    RS232_cputs(c_s_t_setting.u1_cport_nr, s1_str);
    printf("[sent] %d bytes:\n%s", i, s1_str);
    hex(s1_str, i);
    sleep_uart();
    return;
}

unsigned char txMode(unsigned char u1_mode, char *s1_str)
{
    unsigned char u1_break = eON;
    switch(u1_mode)
    {
        case 1:
            txSelect(s1_str);
            break;
        case 2:
            txScanf(s1_str);
            break;
        default:
            printf(" is not found");
            u1_break = eOFF;
            break;
    }
    return u1_break;
}

void discriptionMode(void)
{
    printf("\n/////////////////////////////////////////////////////////////////////////\n");
    printf("what tx mode?\n1 : select mode\n2 : scan mode");
    printf("\n/////////////////////////////////////////////////////////////////////////\n");
    return;
}

void discriptionSelect(void)
{
    unsigned char i;
    printf("\n/////////////////////////////////////////////////////////////////////////\n");
    printf("what at selection?\n");
    for(i = 0; i < c_s_u1_numof_at; i++)
    {
        printf("%d : %s\n",i, c_s_t_at[i].s1_discription);
    }
    printf("/////////////////////////////////////////////////////////////////////////\n");
    return;
}
void discriptionScan(void)
{
    printf("\n/////////////////////////////////////////////////////////////////////////\n");
    printf("what send data?");
    printf("\n/////////////////////////////////////////////////////////////////////////\n");
    return;
}


void MakeTxCommand(char *s1_str, char *s1_ope)
{
    unsigned char u1_ope = (unsigned char)atoi(s1_ope);
    // strcat(s1_str, "AT");
    if(u1_ope < c_s_u1_numof_at)
    {
        strcat(s1_str ,c_s_t_at[u1_ope].s1_command);
        printf("%d : %s\n",u1_ope, c_s_t_at[u1_ope].s1_discription);
    }
    else
    {
        printf("(non_command)\n");
    }
    /* opeの値別にコマンドを挿入する */
    strcat(s1_str, "\r\n");

    return;
}

void txSelect(char *s1_str)
{
    char s1_scan[10];

    discriptionSelect();
    printf("select data : ");
    scanf("%s,", s1_scan);
    MakeTxCommand(s1_str, s1_scan);
    return;
}

void txScanf(char *s1_str)
{
    discriptionScan();
    printf("send data : ");
    scanf("%s",s1_str);
    strcat(s1_str, "\r\n");
    return;
}

void rx(void)
{
    unsigned short n;
    unsigned char buf[4096];

    while(1)
    {
        n = RS232_PollComport(c_s_t_setting.u1_cport_nr, buf, 4095);
        if(n > 0)
        {
            buf[n] = 0;   /* always put a "null" at the end of a string! */
            printf("\n\n[received] %d bytes:\n%s", n, (char *)buf);
            hex((char *)buf, n);
            if(vdChkSuffix((char *) buf))
            {
                break;
            }
        }
    }
    return;
}

unsigned char vdChkSuffix(char *u1_str)
{
    const char *c_s1_err_words[3] = {"OK\r\n", "ERROR\r\n", "FAIL\r\n"};
    for(unsigned char u1_cnt = 0; u1_cnt < 3; u1_cnt++)
    {
        if(NULL != strstr(u1_str, c_s1_err_words[u1_cnt]))
        {
            return eON;
        }
    }
    return eOFF;
}


void hex(char *s1_str, unsigned char n)
{
    printf("hex : ");
    for(unsigned char i = 0; i < n; i++)
    {
        printf("0x%x ", *(s1_str++));
    }
    return;
}


int main()
{
    if(eOK != init())
    {
        return(0);
    }
    while(1)
    {
        tx();
        rx();
    }
  return(0);
}

