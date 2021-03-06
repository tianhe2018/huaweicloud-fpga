/*-
 *   BSD LICENSE
 *
 *   Copyright(c)  2017 Huawei Technologies Co., Ltd. All rights reserved.
 *   All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *     * Neither the name of Huawei Technologies Co., Ltd  nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include "regs_func.h"
#include "pci_rw_tool_func.h"

static int parse_arg(int argc, char* argv[]);
static void help();

static unsigned int g_add_values[2] = {0};
static unsigned int g_port_id = 0;

int main(int argc, char* argv[]) {
	int ret = 0;

	if (0 != parse_arg(argc, argv)) {
		return -EINVAL;
	}
    
	ret = pci_bar2_init_env(g_port_id);
	if (ret != 0) {
		printf("%s: pci_bar2_init_env failed(%d)\r\n", __FUNCTION__, ret);
		return ret;
	}
	
	
	printf("Set [0x%08x]:[0x%08x] to REG_PF_DEMO1_ADDER_CFG_WDATA0:REG_PF_DEMO1_ADDER_CFG_WDATA1 \r\n", g_add_values[0],g_add_values[1]);
	(void)set_add_data(g_add_values[0], g_add_values[1]);

	(void)pci_bar2_uninit_env();
    
	return 0;
}

#define	STR_PARSE_ARG	"i:p:h"
static int parse_arg(int argc, char* argv[]) {
	char*   arg_val = NULL;
	int     ch;
	int add_loop = 0;
	while ((ch=getopt(argc, argv, STR_PARSE_ARG)) != -1) {
        switch (ch) {
            case 'i': {
    		    if (add_loop >= 2) {
                    goto parse_error;        
    		    }
                assert(NULL != optarg);		
                arg_val = optarg;
                g_add_values[add_loop] = strtoul(arg_val, NULL, 16);
    			++add_loop;
                break;
            }
            
            case 'p': {
                assert(NULL != optarg);
                arg_val = optarg;
                g_port_id = strtoul(arg_val, NULL, 0);
                break;
            }
            
            case 'h':
            default:
                goto parse_error;
            
		}
	}
	return 0;
    
parse_error:
    help();
	return -EINVAL;
}

static void help() {
    printf(
        "-----------------------------------------------------------------------------------\r\n"
        "argument format: -i 0xVVV -i 0xPPP [-p port_index]\r\n"
        "\tVVV/PPP: value to be set in REG_PF_DEMO1_ADDER_CFG_WDATA0/REG_PF_DEMO1_ADDER_CFG_WDATA1 register\r\n"
        "\tport_index: the VF's index, 0 as default\r\n"
        "\t-h: print help\r\n"
        "-----------------------------------------------------------------------------------\r\n"
        );
}
