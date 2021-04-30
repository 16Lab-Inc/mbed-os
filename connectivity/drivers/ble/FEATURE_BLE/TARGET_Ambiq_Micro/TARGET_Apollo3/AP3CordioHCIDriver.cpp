/* 
 * Copyright (c) 2020 SparkFun Electronics
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "AP3CordioHCIDriver.h"
#include "AP3CordioHCITransportDriver.h"
#include "am_mcu_apollo.h"
#include "stdio.h"
#include <cstring>

#include "wsf_types.h"
#include "wsf_timer.h"
#include "bstream.h"
#include "wsf_msg.h"
#include "wsf_cs.h"
#include "hci_cmd.h"
#include "hci_api.h"
#include "hci_core.h"

#include "hci_drv_apollo3.h"

using namespace ble;

/* Number of times to send HCI_RAND_CMD on reset to prefill random buffer */
#define HCI_RESET_RAND_CNT        4

AP3CordioHCIDriver::AP3CordioHCIDriver(CordioHCITransportDriver &transport_driver)
    : CordioHCIDriver(transport_driver)
{
    AP3CordioHCITransportDriver *p_trspt_drv = (AP3CordioHCITransportDriver *)&transport_driver;
    _ptr_to_handle = &p_trspt_drv->handle;
}

AP3CordioHCIDriver::~AP3CordioHCIDriver() {}

/*************************************************************************************************/
/*!
 *  \fn     hciCoreReadMaxDataLen
 *
 *  \brief  Read maximum data length command.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void hciCoreReadMaxDataLen(void)
{
  /* if LE Data Packet Length Extensions is supported by Controller and included */
  if ((hciCoreCb.leSupFeat & HCI_LE_SUP_FEAT_DATA_LEN_EXT) &&
      (hciLeSupFeatCfg & HCI_LE_SUP_FEAT_DATA_LEN_EXT))
  {
    /* send next command in sequence */
    HciLeReadMaxDataLen();
  }
  else
  {
    /* send next command in sequence */
    HciLeRandCmd();
  }
}

static void hciCoreReadResolvingListSize(void)
{
  /* if LL Privacy is supported by Controller and included */
  if ((hciCoreCb.leSupFeat & HCI_LE_SUP_FEAT_PRIVACY) &&
      (hciLeSupFeatCfg & HCI_LE_SUP_FEAT_PRIVACY))
  {
    /* send next command in sequence */
    HciLeReadResolvingListSize();
  }
  else
  {
    hciCoreCb.resListSize = 0;

    /* send next command in sequence */
    hciCoreReadMaxDataLen();
  }
}

void AP3CordioHCIDriver::do_initialize()
{
#ifdef USE_AMBIQ_DRIVER
    HciDrvRadioBoot(true);
#else
    MBED_ASSERT(*_ptr_to_handle);
    _ble_config = am_hal_ble_default_config;
    MBED_ASSERT(am_hal_ble_power_control(*_ptr_to_handle, AM_HAL_BLE_POWER_ACTIVE) == AM_HAL_STATUS_SUCCESS);
    MBED_ASSERT(am_hal_ble_config(*_ptr_to_handle, &_ble_config) == AM_HAL_STATUS_SUCCESS);
    MBED_ASSERT(am_hal_ble_boot(*_ptr_to_handle) == AM_HAL_STATUS_SUCCESS);
    MBED_ASSERT(am_hal_ble_tx_power_set(*_ptr_to_handle, 0x0F) == AM_HAL_STATUS_SUCCESS);
    MBED_ASSERT(am_hal_ble_sleep_set(*_ptr_to_handle, false) == AM_HAL_STATUS_SUCCESS);
    am_hal_ble_int_enable(*_ptr_to_handle, (AP3_STUPID_DEF_OF_BLECIRQ_BIT | AM_HAL_BLE_INT_ICMD | AM_HAL_BLE_INT_BLECSSTAT));
    NVIC_EnableIRQ(BLE_IRQn);
#endif
}
void AP3CordioHCIDriver::do_terminate()
{
#ifdef USE_AMBIQ_DRIVER
    HciDrvRadioShutdown();
#else
    am_hal_ble_power_control(*_ptr_to_handle, AM_HAL_BLE_POWER_OFF);
#endif
}

ble::buf_pool_desc_t AP3CordioHCIDriver::get_buffer_pool_description()
{
    static union {
        uint8_t buffer[9000];
        uint64_t align;
    };
    static const wsfBufPoolDesc_t pool_desc[] = {
        {16, 64},
        {32, 64},
        {64, 32},
        {128, 16},
        {272, 4}};
    return buf_pool_desc_t(buffer, pool_desc);
}

ble::CordioHCIDriver &ble_cordio_get_hci_driver()
{
    static AP3CordioHCITransportDriver transport_driver;

    static AP3CordioHCIDriver hci_driver(transport_driver);

    return hci_driver;
}

void AP3CordioHCIDriver::start_reset_sequence()
{
    /* send an HCI Reset command to start the sequence */
    HciResetCmd();

    uint8_t g_BLEMacAddress[6];
    am_hal_mcuctrl_device_t sDevice;
    am_hal_mcuctrl_info_get(AM_HAL_MCUCTRL_INFO_DEVICEID, &sDevice);
    g_BLEMacAddress[0] = sDevice.ui32ChipID0;
    g_BLEMacAddress[1] = sDevice.ui32ChipID0 >> 8;
    g_BLEMacAddress[2] = sDevice.ui32ChipID0 >> 16;

    // update Bluetooth Address to controller
    HciVendorSpecificCmd(0xFC32, 6, g_BLEMacAddress);     
}

void AP3CordioHCIDriver::handle_reset_sequence(uint8_t *pMsg)
{
    uint16_t       opcode;
    wsfMsgHdr_t    hdr;
    static uint8_t randCnt;

  /* if event is a command complete event */
  if (*pMsg == HCI_CMD_CMPL_EVT)
  {
    /* parse parameters */
    pMsg += HCI_EVT_HDR_LEN;
    pMsg++;                   /* skip num packets */
    BSTREAM_TO_UINT16(opcode, pMsg);
    pMsg++;                   /* skip status */

    /* decode opcode */
    switch (opcode)
    {
      case HCI_OPCODE_RESET:
        /* initialize rand command count */
        randCnt = 0;
        /* send next command in sequence */
        HciSetEventMaskCmd((uint8_t *) hciEventMask);
        break;

      case HCI_OPCODE_SET_EVENT_MASK:
        /* send next command in sequence */
        HciLeSetEventMaskCmd((uint8_t *) hciLeEventMask);
        break;

      case HCI_OPCODE_LE_SET_EVENT_MASK:
        /* send next command in sequence */
        HciSetEventMaskPage2Cmd((uint8_t *) hciEventMaskPage2);
        break;

      case HCI_OPCODE_SET_EVENT_MASK_PAGE2:
        /* send next command in sequence */
        HciReadBdAddrCmd();
        break;

      case HCI_OPCODE_READ_BD_ADDR:
        /* parse and store event parameters */
        BdaCpy(hciCoreCb.bdAddr, pMsg);

        /* send next command in sequence */
        HciLeReadBufSizeCmd();
        break;

      case HCI_OPCODE_LE_READ_BUF_SIZE:
        /* parse and store event parameters */
        BSTREAM_TO_UINT16(hciCoreCb.bufSize, pMsg);
        BSTREAM_TO_UINT8(hciCoreCb.numBufs, pMsg);

        if (!APOLLO3_GE_B0)
        {
          hciCoreCb.numBufs--;
        }

        hciCoreCb.availBufs = hciCoreCb.numBufs;

        /* send next command in sequence */
        HciLeReadSupStatesCmd();
        break;

      case HCI_OPCODE_LE_READ_SUP_STATES:
        /* parse and store event parameters */
        memcpy(hciCoreCb.leStates, pMsg, HCI_LE_STATES_LEN);

        /* send next command in sequence */
        HciLeReadWhiteListSizeCmd();
        break;

      case HCI_OPCODE_LE_READ_WHITE_LIST_SIZE:
        /* parse and store event parameters */
        BSTREAM_TO_UINT8(hciCoreCb.whiteListSize, pMsg);

        /* send next command in sequence */
        HciLeReadLocalSupFeatCmd();
        break;

      case HCI_OPCODE_LE_READ_LOCAL_SUP_FEAT:
        /* parse and store event parameters */
        BSTREAM_TO_UINT16(hciCoreCb.leSupFeat, pMsg);

        /* send next command in sequence */
        hciCoreReadResolvingListSize();
        break;

      case HCI_OPCODE_LE_READ_RES_LIST_SIZE:
        /* parse and store event parameters */
        BSTREAM_TO_UINT8(hciCoreCb.resListSize, pMsg);

        /* send next command in sequence */
        hciCoreReadMaxDataLen();
        break;

      case HCI_OPCODE_LE_READ_MAX_DATA_LEN:
        {
          uint16_t maxTxOctets;
          uint16_t maxTxTime;

          BSTREAM_TO_UINT16(maxTxOctets, pMsg);
          BSTREAM_TO_UINT16(maxTxTime, pMsg);

          /* use Controller's maximum supported payload octets and packet duration times
           * for transmission as Host's suggested values for maximum transmission number
           * of payload octets and maximum packet transmission time for new connections.
           */
          HciLeWriteDefDataLen(maxTxOctets, maxTxTime);
        }
        break;

      case HCI_OPCODE_LE_WRITE_DEF_DATA_LEN:
        if (hciCoreCb.extResetSeq)
        {
          /* send first extended command */
          (*hciCoreCb.extResetSeq)(pMsg, opcode);
        }
        else
        {
          /* initialize extended parameters */
          hciCoreCb.maxAdvDataLen = 0;
          hciCoreCb.numSupAdvSets = 0;
          hciCoreCb.perAdvListSize = 0;

          /* send next command in sequence */
          HciLeRandCmd();
        }
        break;

      case HCI_OPCODE_LE_READ_MAX_ADV_DATA_LEN:
      case HCI_OPCODE_LE_READ_NUM_SUP_ADV_SETS:
      case HCI_OPCODE_LE_READ_PER_ADV_LIST_SIZE:
      case HCI_OPCODE_READ_LOCAL_VER_INFO:
        if (hciCoreCb.extResetSeq)
        {
          /* send next extended command in sequence */
          (*hciCoreCb.extResetSeq)(pMsg, opcode);
        }
        break;

      case HCI_OPCODE_LE_RAND:
        /* check if need to send second rand command */
        if (randCnt < (HCI_RESET_RAND_CNT-1))
        {
          randCnt++;
          HciLeRandCmd();
        }
        else
        {
          /* last command in sequence; set resetting state and call callback */
          signal_reset_sequence_done();
        }
        break;

      default:
        break;
        }
    }
}