#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pcap.h>

#include "include/ps_cmdline.h"
#include "include/ps_config.h"
#include "include/ps_context.h"
#include "include/ps_error.h"
#include "include/ps_pkthandler.h"
#include "include/ps_types.h"
#include "include/ps_util.h"

#define PCAP_SNAPLEN 65535
#define PCAP_PROMISC 1

int main(int argc, char** argv)
{
  ps_error_t   error;
  ps_config_t  config;
  ps_context_t context;

  ps_config_init (&config);
  ps_error_clear (&error);
  ps_context_init(&context, &config, &error);

  if (ps_cmdline_parse(argc, argv, &config, &error))
    ps_error_log(&error);

  char pcaperrbuf[PCAP_ERRBUF_SIZE];

  if (!config.interface)
  {
    pcap_if_t *devices, *deviceptr;

    if (pcap_findalldevs(&devices, pcaperrbuf) == PCAP_ERROR)
    {
      ps_error_set(
        &error,
        PS_ERROR_LEVEL_WARN,
        PS_ERROR_TYPE_PCAP,
        pcaperrbuf
      );
      ps_error_log(&error);
    }
    else
    {
      int nchoice, choice, nscan;
      bool is_choice_invalid;

      for (deviceptr = devices, nchoice = 0; deviceptr != NULL; deviceptr = deviceptr->next, nchoice++)
      {
        printf("[%2d]: %s\n", nchoice, deviceptr->name);
      }

      do
      {
        is_choice_invalid = false;
        printf("choice> ");
        nscan = scanf("%d", &choice);
        if (!nscan)
        {
          is_choice_invalid = true;
          ps_error_set(
            &error,
            PS_ERROR_LEVEL_WARN,
            PS_ERROR_TYPE_IO,
            "select device: expected integer value"
          );
          ps_error_log(&error);
        }
        else if (choice < 0 || choice > nchoice - 1)
        {
          is_choice_invalid = true;
          ps_error_set(
            &error,
            PS_ERROR_LEVEL_WARN,
            PS_ERROR_TYPE_IO,
            "select device: choice \"%d\" out of range [0, %d]",
            choice,
            nchoice - 1
          );
          ps_error_log(&error);
        }
        flush_stdin();
      }
      while (is_choice_invalid);

      deviceptr = devices;
      for (int i = 0; i < choice; i++)
        deviceptr = deviceptr->next;

      size_t devnamelen = strlen(deviceptr->name);
      config.interface = malloc(devnamelen + 1);
      if (!config.interface)
      {
        pcap_freealldevs(devices);
        ps_error_set(
          &error,
          PS_ERROR_LEVEL_FATAL,
          PS_ERROR_TYPE_INTERNAL,
          strerror(errno)
        );
        ps_error_log(&error);
      }
    
      config._interface_allocated = 1;
      strcpy(config.interface, deviceptr->name);
      config.interface[devnamelen] = '\0';

      pcap_freealldevs(devices);
    }
  }

  pcap_t* pcaphandle = pcap_create(config.interface, pcaperrbuf);
  if (!pcaphandle)
  {
    ps_error_set(
      &error,
      PS_ERROR_LEVEL_FATAL,
      PS_ERROR_TYPE_PCAP,
      pcaperrbuf
    );
    ps_error_log(&error);
  }

  pcap_set_snaplen(pcaphandle, PCAP_SNAPLEN);
  pcap_set_promisc(pcaphandle, PCAP_PROMISC);
  pcap_set_timeout(pcaphandle, config.timeoutms);


  int rc = pcap_activate(pcaphandle);
  if (rc > 0)
  {
    ps_error_set(
      &error,
      PS_ERROR_LEVEL_WARN,
      PS_ERROR_TYPE_PCAP,
      pcap_geterr(pcaphandle)
    );
    ps_error_log(&error);
  }
  else if (rc < 0)
  {
    ps_error_set(
      &error,
      PS_ERROR_LEVEL_FATAL,
      PS_ERROR_TYPE_PCAP,
      pcap_geterr(pcaphandle)
    );
    ps_error_log(&error);
  }

  config.linktype = pcap_datalink(pcaphandle);
  if (config.linktype != DLT_EN10MB)
  {
    ps_error_set(
      &error,
      PS_ERROR_LEVEL_FATAL,
      PS_ERROR_TYPE_INTERNAL,
      "currently only ethernet is supported"
    );
    ps_error_log(&error);
  }

  if (config.log)
    printf("listening on interface \"%s\"\n", config.interface);

  rc = pcap_loop(pcaphandle, config.packetcount, ps_pkthandler, (uchar*) &context);

  if (config._interface_allocated)
    free(config.interface);

  if (rc)
  {
    ps_error_set(
      &error,
      PS_ERROR_LEVEL_FATAL,
      PS_ERROR_TYPE_PCAP,
      pcap_geterr(pcaphandle)
    );
    ps_error_log(&error);
  }

  return 0;
}
