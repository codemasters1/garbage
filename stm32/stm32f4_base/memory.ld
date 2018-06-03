###############################################################################
# Chip: STM32F417VE
#

MEMORY
{
	rom (rx)		: org = 0x08000000, len = 512k
	ram (rwx)		: org = 0x20000000, len = 112k
	aux_ram (rwx)	: org = 0x2001C000, len = 16k
	ccm_ram (rwx)	: org = 0x10000000, len = 64k
	bkp_ram (rwx)	: org = 0x40024000, len = 4k
}

_eram = ORIGIN(ram) + LENGTH(ram);


###############################################################################
# Definitions of starts, ends and sizes of each memory section.

__rom_start = ORIGIN(rom);
__rom_size = LENGTH(rom);
__rom_end = __rom_start + __rom_size;

__ram_start = ORIGIN(ram);
__ram_size = LENGTH(ram);
__ram_end = __ram_start + __ram_size;

__aux_ram_start = ORIGIN(aux_ram);
__aux_ram_size = LENGTH(aux_ram);
__aux_ram_end = __aux_ram_start + __aux_ram_size;

__ccm_ram_start = ORIGIN(ccm_ram);
__ccm_ram_size = LENGTH(ccm_ram);
__ccm_ram_end = __ccm_ram_start + __ccm_ram_size;

__bkp_ram_start = ORIGIN(bkp_ram);
__bkp_ram_size = LENGTH(bkp_ram);
__bkp_ram_end = __bkp_ram_start + __bkp_ram_size;


###############################################################################
# Definitions which may be used in application.
# Note that it would be used only if their names are unique.

PROVIDE(__rom_start = __rom_start);
PROVIDE(__rom_size = __rom_size);
PROVIDE(__rom_end = __rom_end);

PROVIDE(__ram_start = __ram_start);
PROVIDE(__ram_size = __ram_size);
PROVIDE(__ram_end = __ram_end);

PROVIDE(__aux_ram_start = __aux_ram_start);
PROVIDE(__aux_ram_size = __aux_ram_size);
PROVIDE(__aux_ram_end = __aux_ram_end);

PROVIDE(__ccm_ram_start = __ccm_ram_start);
PROVIDE(__ccm_ram_size = __ccm_ram_size);
PROVIDE(__ccm_ram_end = __ccm_ram_end);

PROVIDE(__bkp_ram_start = __bkp_ram_start);
PROVIDE(__bkp_ram_size = __bkp_ram_size);
PROVIDE(__bkp_ram_end = __bkp_ram_end);