# Copyright 2022 Barcelona Supercomputing Center-Centro Nacional de Supercomputación

# Licensed under the Solderpad Hardware License v 2.1 (the "License");
# you may not use this file except in compliance with the License, or, at your option, the Apache License version 2.0.
# You may obtain a copy of the License at
# 
#     http://www.solderpad.org/licenses/SHL-2.1
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Author: Alexander Kropotov, BSC-CNS
# Date: 22.02.2022
# Description: 



set g_eth100gb_freq "161.1328125"

  set eth100gb [ create_bd_cell -type ip -vlnv xilinx.com:ip:cmac_usplus:3.1 eth100gb ]
  set_property -dict [ list \
   CONFIG.ADD_GT_CNRL_STS_PORTS {0} \
   CONFIG.CMAC_CAUI4_MODE {1} \
   CONFIG.CMAC_CORE_SELECT {CMACE4_X0Y3} \
   CONFIG.DIFFCLK_BOARD_INTERFACE {Custom} \
   CONFIG.ENABLE_AXI_INTERFACE {1} \
   CONFIG.ENABLE_PIPELINE_REG {0} \
   CONFIG.ENABLE_TIME_STAMPING {0} \
   CONFIG.ETHERNET_BOARD_INTERFACE {Custom} \
   CONFIG.GT_GROUP_SELECT {X0Y24~X0Y27} \
   CONFIG.GT_REF_CLK_FREQ $g_eth100gb_freq \
   CONFIG.GT_RX_BUFFER_BYPASS {0} \
   CONFIG.INCLUDE_AUTO_NEG_LT_LOGIC {0} \
   CONFIG.INCLUDE_RS_FEC {1} \
   CONFIG.INCLUDE_STATISTICS_COUNTERS {1} \
   CONFIG.LANE10_GT_LOC {NA} \
   CONFIG.LANE1_GT_LOC {X0Y24} \
   CONFIG.LANE2_GT_LOC {X0Y25} \
   CONFIG.LANE3_GT_LOC {X0Y26} \
   CONFIG.LANE4_GT_LOC {X0Y27} \
   CONFIG.LANE5_GT_LOC {NA} \
   CONFIG.LANE6_GT_LOC {NA} \
   CONFIG.LANE7_GT_LOC {NA} \
   CONFIG.LANE8_GT_LOC {NA} \
   CONFIG.LANE9_GT_LOC {NA} \
   CONFIG.NUM_LANES {4x25} \
   CONFIG.PLL_TYPE {QPLL0} \
   CONFIG.RX_CHECK_ACK {1} \
   CONFIG.RX_EQ_MODE {AUTO} \
   CONFIG.RX_FLOW_CONTROL {0} \
   CONFIG.RX_FORWARD_CONTROL_FRAMES {0} \
   CONFIG.RX_GT_BUFFER {1} \
   CONFIG.RX_MAX_PACKET_LEN {9600} \
   CONFIG.RX_MIN_PACKET_LEN {64} \
   CONFIG.TX_FLOW_CONTROL {0} \
   CONFIG.TX_OTN_INTERFACE {0} \
   CONFIG.USER_INTERFACE {AXIS} \
   CONFIG.USE_BOARD_FLOW {true} \
 ] $eth100gb
