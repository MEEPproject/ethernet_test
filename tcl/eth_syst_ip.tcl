# Copyright 2022 Barcelona Supercomputing Center-Centro Nacional de Supercomputación

# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Author: Alexander Kropotov, BSC-CNS
# Date: 22.02.2022
# Description: 



# Script to generate IP core of isolated Ethernet subsystem
#
#

#source $root_dir/tcl/environment.tcl
source $g_root_dir/tcl/project_options.tcl


set ip_properties [ list \
    vendor "meep-project.eu" \
    library "MEEP" \
    name "MEEP_100Gb_Ethernet" \
    version "$g_ip_version" \
    taxonomy "/MEEP_IP" \
    display_name "MEEP 100Gb Ethernet" \
    description "100Gb Ethernet standalone design" \
        core_revision 1 \
    vendor_display_name "bsc.es" \
    company_url "https://meep-project.eu/" \
    ]

set family_lifecycle { \
  virtexuplusHBM Production \
}


ipx::package_project -root_dir $g_root_dir/ip -module Eth_CMAC_syst -import_files

set ip_core [ipx::current_core]
set_property -dict ${ip_properties} ${ip_core}
set_property SUPPORTED_FAMILIES ${family_lifecycle} ${ip_core}

## Relative path to IP root directory
ipx::create_xgui_files ${ip_core} -logo_file "$g_root_dir/misc/BSC-Logo.png"
set_property type LOGO [ipx::get_files "$g_root_dir/misc/BSC-Logo.png" -of_objects [ipx::get_file_groups xilinx_utilityxitfiles -of_objects [ipx::current_core]]]
ipx::update_checksums ${ip_core}

# Save IP and close project
ipx::check_integrity ${ip_core}
ipx::save_core ${ip_core}

ipx::archive_core $g_root_dir/ip/${g_design_name}.zip
ipx::unload_core $g_root_dir/ip/component.xml

update_ip_catalog -rebuild

puts "IP succesfully packaged "
