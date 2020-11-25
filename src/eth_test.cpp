
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// #include <vector>
// #include <string>
// #include <xil_sleeptimer.h>
// #include <cassert>
// #include <iostream>
// #include <fstream>
// #include <sstream>
// #include <thread>
// #include <chrono>
// #include <fcntl.h>
// #include <sys/stat.h>

#include "xparameters.h"
#include "../../../src/ethernet_test_cmac_usplus_0_0_axi4_lite_registers.h"
#include "fsl.h" // FSL macros: https://www.xilinx.com/support/documentation/sw_manuals/xilinx2016_4/oslib_rm.pdf#page=16

// using namespace std;

void transmitToChan(uint8_t chanDepth, bool rxCheck) {
    printf("Transmitting data to channel with depth %d:\n", chanDepth);
    uint32_t putData = 0xDEADBEEF;
    uint32_t getData = 0;
    bool fslNrdy = true;
    bool fslErr  = true;

    if (rxCheck) {
      // if initially Rx channel should be empty
      getfslx(getData, 0, FSL_NONBLOCKING);
      fsl_isinvalid(fslNrdy);
      fsl_iserror  (fslErr);
      if (!fslNrdy) {
        printf("\nERROR: Before starting filling Tx channel Rx channel is not empty: FSL0 = %0lX, Empty = %d, Err = %d \n",
                 getData, fslNrdy, fslErr);
        exit(1);
      }
    }

    for (uint8_t word = 0; word < chanDepth;                 word++)
    for (uint8_t chan = 0; chan < XPAR_MICROBLAZE_FSL_LINKS; chan++) {
      // FSL id goes to macro as literal
      if (word%2) { // transmitting TLAST in odd words (FSL 0 only is used to pass this control)
        if (chan==0)  putfslx(putData,  0, FSL_NONBLOCKING_CONTROL);
        if (chan==1)  putfslx(putData,  1, FSL_NONBLOCKING);
        if (chan==2)  putfslx(putData,  2, FSL_NONBLOCKING);
        if (chan==3)  putfslx(putData,  3, FSL_NONBLOCKING);
        if (chan==4)  putfslx(putData,  4, FSL_NONBLOCKING);
        if (chan==5)  putfslx(putData,  5, FSL_NONBLOCKING);
        if (chan==6)  putfslx(putData,  6, FSL_NONBLOCKING);
        if (chan==7)  putfslx(putData,  7, FSL_NONBLOCKING);
        if (chan==8)  putfslx(putData,  8, FSL_NONBLOCKING);
        if (chan==9)  putfslx(putData,  9, FSL_NONBLOCKING);
        if (chan==10) putfslx(putData, 10, FSL_NONBLOCKING);
        if (chan==11) putfslx(putData, 11, FSL_NONBLOCKING);
        if (chan==12) putfslx(putData, 12, FSL_NONBLOCKING);
        if (chan==13) putfslx(putData, 13, FSL_NONBLOCKING);
        if (chan==14) putfslx(putData, 14, FSL_NONBLOCKING);
        if (chan==15) putfslx(putData, 15, FSL_NONBLOCKING);
      } else {
        if (chan==0)  putfslx(putData,  0, FSL_NONBLOCKING);
        if (chan==1)  putfslx(putData,  1, FSL_NONBLOCKING_CONTROL);
        if (chan==2)  putfslx(putData,  2, FSL_NONBLOCKING_CONTROL);
        if (chan==3)  putfslx(putData,  3, FSL_NONBLOCKING_CONTROL);
        if (chan==4)  putfslx(putData,  4, FSL_NONBLOCKING_CONTROL);
        if (chan==5)  putfslx(putData,  5, FSL_NONBLOCKING_CONTROL);
        if (chan==6)  putfslx(putData,  6, FSL_NONBLOCKING_CONTROL);
        if (chan==7)  putfslx(putData,  7, FSL_NONBLOCKING_CONTROL);
        if (chan==8)  putfslx(putData,  8, FSL_NONBLOCKING_CONTROL);
        if (chan==9)  putfslx(putData,  9, FSL_NONBLOCKING_CONTROL);
        if (chan==10) putfslx(putData, 10, FSL_NONBLOCKING_CONTROL);
        if (chan==11) putfslx(putData, 11, FSL_NONBLOCKING_CONTROL);
        if (chan==12) putfslx(putData, 12, FSL_NONBLOCKING_CONTROL);
        if (chan==13) putfslx(putData, 13, FSL_NONBLOCKING_CONTROL);
        if (chan==14) putfslx(putData, 14, FSL_NONBLOCKING_CONTROL);
        if (chan==15) putfslx(putData, 15, FSL_NONBLOCKING_CONTROL);
      }
      fsl_isinvalid(fslNrdy);
      fsl_iserror  (fslErr);
      // printf("Writing word %d to FSL%d = %0lX, Full = %d, Err = %d \n", word, chan, putData, fslNrdy, fslErr);
      if (fslNrdy || fslErr) {
        printf("\nERROR: Failed write of word %d to FSL%d = %0lX, Full = %d, Err = %d \n", word, chan, putData, fslNrdy, fslErr);
        exit(1);
      }
      putData += chan;
      putData = ~putData;
    }
    printf("\n");
    // here Tx channel should be full
    putfslx(putData, 0, FSL_NONBLOCKING);
    fsl_isinvalid(fslNrdy);
    if (!fslNrdy) {
      printf("\nERROR: After filling Tx channel it is still not full\n");
      exit(1);
    }
}

void receiveFrChan(uint8_t chanDepth) {
    printf("Receiving data from channel with depth %d:\n", chanDepth);
    uint32_t putData = 0xDEADBEEF;
    uint32_t getData = 0;
    bool fslNrdy = true;
    bool fslErr  = true;

    for (uint8_t word = 0; word < chanDepth     ;            word++)
    for (uint8_t chan = 0; chan < XPAR_MICROBLAZE_FSL_LINKS; chan++) {
      // FSL id goes to macro as literal 
      if (word%2) { // expecting TLAST in odd words (populated to all FSLs)
        if (chan==0)  getfslx(getData,  0, FSL_NONBLOCKING_CONTROL);
        if (chan==1)  getfslx(getData,  1, FSL_NONBLOCKING_CONTROL);
        if (chan==2)  getfslx(getData,  2, FSL_NONBLOCKING_CONTROL);
        if (chan==3)  getfslx(getData,  3, FSL_NONBLOCKING_CONTROL);
        if (chan==4)  getfslx(getData,  4, FSL_NONBLOCKING_CONTROL);
        if (chan==5)  getfslx(getData,  5, FSL_NONBLOCKING_CONTROL);
        if (chan==6)  getfslx(getData,  6, FSL_NONBLOCKING_CONTROL);
        if (chan==7)  getfslx(getData,  7, FSL_NONBLOCKING_CONTROL);
        if (chan==8)  getfslx(getData,  8, FSL_NONBLOCKING_CONTROL);
        if (chan==9)  getfslx(getData,  9, FSL_NONBLOCKING_CONTROL);
        if (chan==10) getfslx(getData, 10, FSL_NONBLOCKING_CONTROL);
        if (chan==11) getfslx(getData, 11, FSL_NONBLOCKING_CONTROL);
        if (chan==12) getfslx(getData, 12, FSL_NONBLOCKING_CONTROL);
        if (chan==13) getfslx(getData, 13, FSL_NONBLOCKING_CONTROL);
        if (chan==14) getfslx(getData, 14, FSL_NONBLOCKING_CONTROL);
        if (chan==15) getfslx(getData, 15, FSL_NONBLOCKING_CONTROL);
      } else {
        if (chan==0)  getfslx(getData,  0, FSL_NONBLOCKING);
        if (chan==1)  getfslx(getData,  1, FSL_NONBLOCKING);
        if (chan==2)  getfslx(getData,  2, FSL_NONBLOCKING);
        if (chan==3)  getfslx(getData,  3, FSL_NONBLOCKING);
        if (chan==4)  getfslx(getData,  4, FSL_NONBLOCKING);
        if (chan==5)  getfslx(getData,  5, FSL_NONBLOCKING);
        if (chan==6)  getfslx(getData,  6, FSL_NONBLOCKING);
        if (chan==7)  getfslx(getData,  7, FSL_NONBLOCKING);
        if (chan==8)  getfslx(getData,  8, FSL_NONBLOCKING);
        if (chan==9)  getfslx(getData,  9, FSL_NONBLOCKING);
        if (chan==10) getfslx(getData, 10, FSL_NONBLOCKING);
        if (chan==11) getfslx(getData, 11, FSL_NONBLOCKING);
        if (chan==12) getfslx(getData, 12, FSL_NONBLOCKING);
        if (chan==13) getfslx(getData, 13, FSL_NONBLOCKING);
        if (chan==14) getfslx(getData, 14, FSL_NONBLOCKING);
        if (chan==15) getfslx(getData, 15, FSL_NONBLOCKING);
      }
      fsl_isinvalid(fslNrdy);
      fsl_iserror  (fslErr);
      // printf("Reading word %d from FSL%d = %0lX, Empty = %d, Err = %d \n", word, chan, getData, fslNrdy, fslErr);
      if (fslNrdy || fslErr || getData!=putData) {
        printf("\nERROR: Failed read of word %d from FSL%d = %0lX (expected %0lX), Empty = %d, Err = %d \n",
               word, chan, getData, putData, fslNrdy, fslErr);
        exit(1);
      }
      putData += chan;
      putData = ~putData;
    }
    printf("\n");
    // here Rx channel should be empty
    getfslx(getData, 0, FSL_NONBLOCKING);
    fsl_isinvalid(fslNrdy);
    fsl_iserror  (fslErr);
    if (!fslNrdy) {
      printf("\nERROR: After reading out Rx channel it is still not empty: FSL0 = %0lX, Empty = %d, Err = %d \n",
               getData, fslNrdy, fslErr);
      exit(1);
    }
}

void switch_EthTrue_LBFalse(bool EthNLB) {
  // AXIS switches control: https://www.xilinx.com/support/documentation/ip_documentation/axis_infrastructure_ip_suite/v1_1/pg085-axi4stream-infrastructure.pdf#page=27
  uint32_t* txSwitch = reinterpret_cast<uint32_t*>(XPAR_TX_AXIS_SWITCH_BASEADDR);
  uint32_t* rxSwitch = reinterpret_cast<uint32_t*>(XPAR_RX_AXIS_SWITCH_BASEADDR);
  enum {MI_MUX = 0x0040 / sizeof(uint32_t)};

  printf("Tx/Rx streams switches state:\n");
  printf("TX Switch: Control = %0lX, Out0 = %0lX, Out1 = %0lX \n", txSwitch[0], txSwitch[MI_MUX], txSwitch[MI_MUX+1]);
  printf("RX Switch: Control = %0lX, Out0 = %0lX \n",              rxSwitch[0], rxSwitch[MI_MUX]);
  printf("Setting switches to Ethernet core mode:\n");
  if (EthNLB) {
    txSwitch[MI_MUX+0] = 0x80000000; // Tx: switching-off Out0(Short loopback)
    txSwitch[MI_MUX+1] = 0;          // Tx: switching     Out1(Ethernet core) to In0
    rxSwitch[MI_MUX]   = 1;          // Rx: switching     Out0 to In1(Ethernet core)
  } else {
    txSwitch[MI_MUX+0] = 0;          // Tx: switching     Out0(Short loopback) to In0
    txSwitch[MI_MUX+1] = 0x80000000; // Tx: switching-off Out1(Ethernet core)
    rxSwitch[MI_MUX]   = 0;          // Rx: switching     Out0 to In0(Short loopback)
  }
  printf("TX Switch: Control = %0lX, Out0 = %0lX, Out1 = %0lX \n", txSwitch[0], txSwitch[MI_MUX], txSwitch[MI_MUX+1]);
  printf("RX Switch: Control = %0lX, Out0 = %0lX \n",              rxSwitch[0], rxSwitch[MI_MUX]);
  printf("Commiting the setting\n");
  txSwitch[0] = 0x2;
  rxSwitch[0] = 0x2;
  printf("TX Control = %0lX, RX Control = %0lX\n", txSwitch[0], rxSwitch[0]);
  printf("TX Switch: Control = %0lX, Out0 = %0lX, Out1 = %0lX \n", txSwitch[0], txSwitch[MI_MUX], txSwitch[MI_MUX+1]);
  printf("RX Switch: Control = %0lX, Out0 = %0lX \n",              rxSwitch[0], rxSwitch[MI_MUX]);
  sleep(1); // in seconds
  printf("\n");
}


int main(int argc, char *argv[])
{

  //100Gb Ethernet subsystem registers: https://www.xilinx.com/support/documentation/ip_documentation/cmac_usplus/v3_1/pg203-cmac-usplus.pdf#page=177
  // uint32_t* ethCore = reinterpret_cast<uint32_t*>(XPAR_CMAC_USPLUS_0_BASEADDR);
  enum {
    GT_RESET_REG          = GT_RESET_REG_OFFSET          / sizeof(uint32_t),
    RESET_REG             = RESET_REG_OFFSET             / sizeof(uint32_t),
    CORE_VERSION_REG      = CORE_VERSION_REG_OFFSET      / sizeof(uint32_t),
    CORE_MODE_REG         = CORE_MODE_REG_OFFSET         / sizeof(uint32_t),
    SWITCH_CORE_MODE_REG  = SWITCH_CORE_MODE_REG_OFFSET  / sizeof(uint32_t),
    CONFIGURATION_TX_REG1 = CONFIGURATION_TX_REG1_OFFSET / sizeof(uint32_t),
    CONFIGURATION_RX_REG1 = CONFIGURATION_RX_REG1_OFFSET / sizeof(uint32_t),
    STAT_TX_STATUS_REG    = STAT_TX_STATUS_REG_OFFSET    / sizeof(uint32_t),
    STAT_RX_STATUS_REG    = STAT_RX_STATUS_REG_OFFSET    / sizeof(uint32_t),
    GT_LOOPBACK_REG       = GT_LOOPBACK_REG_OFFSET       / sizeof(uint32_t)
  };

  // Ethernet core control via GPIO 
  uint32_t* rxtxCtrl = reinterpret_cast<uint32_t*>(XPAR_TX_RX_CTL_STAT_BASEADDR);
  enum {
    TX_CTRL = 0x0000 / sizeof(uint32_t),
    RX_CTRL = 0x0008 / sizeof(uint32_t)
  };
  uint32_t* gtCtrl = reinterpret_cast<uint32_t*>(XPAR_GT_CTL_BASEADDR);

  enum {SHORT_LOOPBACK_DEPTH = 100,
        TRANSMIT_FIFO_DEPTH  = 38
       };


  while (true) {

    printf("\n");
    printf("------ Ethernet Test App ------\n");
    printf("Please enter test mode:\n");
    printf("  Loopback:                       l\n");
    printf("  Communication with other board: c\n");
    printf("  Finish:                         f\n");
    char choice;
    scanf("%s", &choice);
    printf("You have entered: %c\n", choice);


    switch (choice) {
      case 'l': // Loopback test

    printf("------- Running Short Loopback test -------\n");
    switch_EthTrue_LBFalse(false);
    transmitToChan(SHORT_LOOPBACK_DEPTH, true);
    receiveFrChan (SHORT_LOOPBACK_DEPTH);
    printf("------- Short Loopback test passed -------\n\n");


    printf("------- Running Near-end Loopback test -------\n");
    switch_EthTrue_LBFalse(true);

    // printf("Soft reset of Ethernet core:\n");
    // printf("GT_RESET_REG: %0lX, RESET_REG: %0lX \n", ethCore[GT_RESET_REG], ethCore[RESET_REG]);
    // ethCore[GT_RESET_REG] = GT_RESET_REG_GT_RESET_ALL_MASK;
    // ethCore[RESET_REG]    = RESET_REG_USR_RX_SERDES_RESET_MASK |
    //                         RESET_REG_USR_RX_RESET_MASK        |
    //                         RESET_REG_USR_TX_RESET_MASK;
    // printf("GT_RESET_REG: %0lX, RESET_REG: %0lX \n", ethCore[GT_RESET_REG], ethCore[RESET_REG]);
    // printf("GT_RESET_REG: %0lX, RESET_REG: %0lX \n", ethCore[GT_RESET_REG], ethCore[RESET_REG]);
    // sleep(1); // in seconds
    // printf("\n");

    // Reading status via GPIO
    printf("GT_STATUS: %0lX \n", gtCtrl[0]);
    printf("TX_STATUS: %0lX, RX_STATUS: %0lX \n", rxtxCtrl[TX_CTRL], rxtxCtrl[RX_CTRL]);
    // Reading status and other regs via AXI
    // printf("GT_RESET_REG:          %0lX \n", ethCore[GT_RESET_REG]);
    // printf("RESET_REG:             %0lX \n", ethCore[RESET_REG]);
    // printf("CORE_VERSION_REG:      %0lX \n", ethCore[CORE_VERSION_REG]);
    // printf("CORE_MODE_REG:         %0lX \n", ethCore[CORE_MODE_REG]);
    // printf("SWITCH_CORE_MODE_REG:  %0lX \n", ethCore[SWITCH_CORE_MODE_REG]);
    // printf("CONFIGURATION_TX_REG1: %0lX \n", ethCore[CONFIGURATION_TX_REG1]);
    // printf("CONFIGURATION_RX_REG1: %0lX \n", ethCore[CONFIGURATION_RX_REG1]);
    // printf("STAT_TX_STATUS_REG:    %0lX \n", ethCore[STAT_TX_STATUS_REG]);
    // printf("STAT_RX_STATUS_REG:    %0lX \n", ethCore[STAT_RX_STATUS_REG]);
    // printf("GT_LOOPBACK_REG:       %0lX \n", ethCore[GT_LOOPBACK_REG]);

    printf("Enabling Near-End PMA Loopback\n");
    // via GPIO
    gtCtrl[0] = 0x2222; // https://www.xilinx.com/support/documentation/user_guides/ug578-ultrascale-gty-transceivers.pdf#page=88
    // via AXI
    // printf("GT_LOOPBACK_REG: %0lX \n", ethCore[GT_LOOPBACK_REG]);
    // ethCore[GT_LOOPBACK_REG] = GT_LOOPBACK_REG_CTL_GT_LOOPBACK_MASK;
    // printf("GT_LOOPBACK_REG: %0lX \n", ethCore[GT_LOOPBACK_REG]);
    // printf("GT_LOOPBACK_REG: %0lX \n", ethCore[GT_LOOPBACK_REG]);

    printf("Ethernet core bring-up.\n");
    // https://www.xilinx.com/support/documentation/ip_documentation/cmac_usplus/v3_1/pg203-cmac-usplus.pdf#page=204
    // via GPIO
    rxtxCtrl[RX_CTRL] = CONFIGURATION_RX_REG1_CTL_RX_ENABLE_MASK;
    rxtxCtrl[TX_CTRL] = CONFIGURATION_TX_REG1_CTL_TX_SEND_RFI_MASK;
    // via AXI
    // printf("CONFIGURATION_TX(RX)_REG1: %0lX, %0lX\n", ethCore[CONFIGURATION_TX_REG1],
    //                                                   ethCore[CONFIGURATION_RX_REG1]);
    // ethCore[CONFIGURATION_RX_REG1] = CONFIGURATION_RX_REG1_CTL_RX_ENABLE_MASK;
    // ethCore[CONFIGURATION_TX_REG1] = CONFIGURATION_TX_REG1_CTL_TX_SEND_RFI_MASK;
    // printf("CONFIGURATION_TX(RX)_REG1: %0lX, %0lX\n", ethCore[CONFIGURATION_TX_REG1],
    //                                                   ethCore[CONFIGURATION_RX_REG1]);
    // printf("CONFIGURATION_TX(RX)_REG1: %0lX, %0lX\n", ethCore[CONFIGURATION_TX_REG1],
    //                                                   ethCore[CONFIGURATION_RX_REG1]);
                                               
    printf("Waiting for RX is aligned...\n");
    while(!(rxtxCtrl[RX_CTRL] & STAT_RX_STATUS_REG_STAT_RX_ALIGNED_MASK)) {
      printf("TX_STATUS: %0lX, RX_STATUS: %0lX \n", rxtxCtrl[TX_CTRL], rxtxCtrl[RX_CTRL]);
      // printf("STAT_TX(RX)_STATUS_REG: %0lX, %0lX\n", ethCore[STAT_TX_STATUS_REG],
      //                                                ethCore[STAT_RX_STATUS_REG]);
    }
    printf("RX is aligned:\n");
    printf("TX_STATUS: %0lX, RX_STATUS: %0lX \n", rxtxCtrl[TX_CTRL], rxtxCtrl[RX_CTRL]);
    // printf("STAT_TX(RX)_STATUS_REG: %0lX, %0lX\n", ethCore[STAT_TX_STATUS_REG],
    //                                                ethCore[STAT_RX_STATUS_REG]);

    printf("Disabling TX_SEND_RFI:\n");
    // via GPIO
    rxtxCtrl[TX_CTRL] = 0;
    // via AXI
    // ethCore[CONFIGURATION_TX_REG1] = 0;
    printf("Waiting for RFI is stopped...\n");
    while(!(rxtxCtrl[RX_CTRL] & STAT_RX_STATUS_REG_STAT_RX_ALIGNED_MASK) ||
           (rxtxCtrl[RX_CTRL] & STAT_RX_STATUS_REG_STAT_RX_REMOTE_FAULT_MASK)) {
      printf("TX_STATUS: %0lX, RX_STATUS: %0lX \n", rxtxCtrl[TX_CTRL], rxtxCtrl[RX_CTRL]);
      // printf("STAT_TX(RX)_STATUS_REG: %0lX, %0lX\n", ethCore[STAT_TX_STATUS_REG],
      //                                                ethCore[STAT_RX_STATUS_REG]);
    }
    printf("RFI is stopped:\n");
    printf("TX_STATUS: %0lX, RX_STATUS: %0lX \n", rxtxCtrl[TX_CTRL], rxtxCtrl[RX_CTRL]);
    // printf("STAT_TX(RX)_STATUS_REG: %0lX, %0lX\n", ethCore[STAT_TX_STATUS_REG],
    //                                                ethCore[STAT_RX_STATUS_REG]);

    transmitToChan(TRANSMIT_FIFO_DEPTH, true);

    printf("Enabling Ethernet TX:\n");
    // via GPIO
    rxtxCtrl[TX_CTRL] = CONFIGURATION_TX_REG1_CTL_TX_ENABLE_MASK;
    printf("TX_STATUS: %0lX, RX_STATUS: %0lX \n", rxtxCtrl[TX_CTRL], rxtxCtrl[RX_CTRL]);
    printf("TX_STATUS: %0lX, RX_STATUS: %0lX \n", rxtxCtrl[TX_CTRL], rxtxCtrl[RX_CTRL]);
    // via AXI
    // printf("CONFIGURATION_TX_REG1: %0lX \n", ethCore[CONFIGURATION_TX_REG1]);
    // ethCore[CONFIGURATION_TX_REG1] = CONFIGURATION_TX_REG1_CTL_TX_ENABLE_MASK;
    // printf("CONFIGURATION_TX_REG1: %0lX \n", ethCore[CONFIGURATION_TX_REG1]);
    // printf("CONFIGURATION_TX_REG1: %0lX \n", ethCore[CONFIGURATION_TX_REG1]);

    receiveFrChan (TRANSMIT_FIFO_DEPTH);
    
    // Disabling Tx/Rx
    // via GPIO
    rxtxCtrl[TX_CTRL] = 0;
    rxtxCtrl[RX_CTRL] = 0;
    // via AXI
    // printf("CONFIGURATION_TX_REG1: %0lX \n", ethCore[CONFIGURATION_TX_REG1]);
    // ethCore[CONFIGURATION_TX_REG1] = 0;
    // ethCore[CONFIGURATION_RX_REG1] = 0;
    // printf("CONFIGURATION_TX_REG1: %0lX \n", ethCore[CONFIGURATION_TX_REG1]);
    // printf("CONFIGURATION_TX_REG1: %0lX \n", ethCore[CONFIGURATION_TX_REG1]);

    printf("------- Near-end Loopback test passed -------\n\n");
    break;


      case 'c':
    printf("------- Running 2 cards communication test -------\n");
    printf("Please make sure that the app is running on the other side and confirm with 'y'...\n");
    char confirm;
    scanf("%s", &confirm);
    printf("%c\n", confirm);
    if (confirm != 'y') break;

    switch_EthTrue_LBFalse(true);

    printf("GT_STATUS: %0lX \n", gtCtrl[0]);
    printf("TX_STATUS: %0lX, RX_STATUS: %0lX \n", rxtxCtrl[TX_CTRL], rxtxCtrl[RX_CTRL]);

    printf("Enabling GT normal operation with no loopback\n");
    gtCtrl[0] = 0; // https://www.xilinx.com/support/documentation/user_guides/ug578-ultrascale-gty-transceivers.pdf#page=88
    printf("Ethernet core bring-up.\n");
    // https://www.xilinx.com/support/documentation/ip_documentation/cmac_usplus/v3_1/pg203-cmac-usplus.pdf#page=204
    rxtxCtrl[RX_CTRL] = CONFIGURATION_RX_REG1_CTL_RX_ENABLE_MASK;
    rxtxCtrl[TX_CTRL] = CONFIGURATION_TX_REG1_CTL_TX_SEND_RFI_MASK;

    printf("Waiting for RX is aligned and sync-up with other side...\n");
    while(!(rxtxCtrl[RX_CTRL] & STAT_RX_STATUS_REG_STAT_RX_ALIGNED_MASK) ||
          !(rxtxCtrl[RX_CTRL] & STAT_RX_STATUS_REG_STAT_RX_REMOTE_FAULT_MASK)) {
      printf("TX_STATUS: %0lX, RX_STATUS: %0lX \n", rxtxCtrl[TX_CTRL], rxtxCtrl[RX_CTRL]);
      sleep(1); // in seconds, user wait process
    }
    printf("RX is aligned and RFI is got from the other side:\n");
    printf("TX_STATUS: %0lX, RX_STATUS: %0lX \n", rxtxCtrl[TX_CTRL], rxtxCtrl[RX_CTRL]);

    printf("Disabling TX_SEND_RFI:\n");
    sleep(1); // in seconds, timeout to make sure opposite side got RFI
    rxtxCtrl[TX_CTRL] = 0;

    printf("Waiting for RFI is stopped...\n");
    while(!(rxtxCtrl[RX_CTRL] & STAT_RX_STATUS_REG_STAT_RX_ALIGNED_MASK) ||
           (rxtxCtrl[RX_CTRL] & STAT_RX_STATUS_REG_STAT_RX_REMOTE_FAULT_MASK)) {
      printf("TX_STATUS: %0lX, RX_STATUS: %0lX \n", rxtxCtrl[TX_CTRL], rxtxCtrl[RX_CTRL]);
      sleep(1); // in seconds, user wait process
    }
    printf("RFI is stopped:\n");
    printf("TX_STATUS: %0lX, RX_STATUS: %0lX \n", rxtxCtrl[TX_CTRL], rxtxCtrl[RX_CTRL]);

    transmitToChan(TRANSMIT_FIFO_DEPTH, false);
    printf("Enabling Ethernet TX:\n");
    rxtxCtrl[TX_CTRL] = CONFIGURATION_TX_REG1_CTL_TX_ENABLE_MASK;
    printf("TX_STATUS: %0lX, RX_STATUS: %0lX \n", rxtxCtrl[TX_CTRL], rxtxCtrl[RX_CTRL]);
    printf("TX_STATUS: %0lX, RX_STATUS: %0lX \n", rxtxCtrl[TX_CTRL], rxtxCtrl[RX_CTRL]);
    sleep(1); // in seconds, delay not to use blocking read in receive process
    receiveFrChan (TRANSMIT_FIFO_DEPTH);
    // Disabling Tx/Rx
    rxtxCtrl[TX_CTRL] = 0;
    rxtxCtrl[RX_CTRL] = 0;

    printf("------- 2 cards communication test passed -------\n\n");
    break;

      case 'f':
        printf("------- Exiting the app -------\n");
        return(0) ;

      default:
        printf("Please choose right option\n");
    }
  }
}
