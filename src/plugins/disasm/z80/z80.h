#ifndef __ZILOG_Z80
#define __ZILOG_Z80 1

#include "config.h"
#include "libbeye/libbeye.h"
#include "plugins/disasm.h"
#include "beyehelp.h"

using namespace	usr;
/**
 * @namespace	usr_plugins_II
 * @file        plugins/disasm/z80.cpp
 * @brief       This file contains implementation of Z80 disassembler.
 * @version     -
 * @remark      this source file is part of Binary EYE project (BEYE).
 *              The Binary EYE (BEYE) is copyright (C) 1995 Nickols_K.
 *              All rights reserved. This software is redistributable under the
 *              licence given in the file "Licence.en" ("Licence.ru" in russian
 *              translation) distributed in the BEYE archive.
 * @note        Requires POSIX compatible development system
 *
 * @author      mrmks04
 * @since       2017
 * @note        Development, fixes and improvements
**/

namespace	usr {

    class Z80_Disassembler : public Disassembler {
	public:
	    Z80_Disassembler(BeyeContext& bc,const Bin_Format& b,binary_stream& h,DisMode& parent);
	    virtual ~Z80_Disassembler();
	
	    virtual const char*	prompt(unsigned idx) const;
	    virtual bool	action_F1();

	    virtual DisasmRet	disassembler(__filesize_t shift,MBuffer insn_buff,unsigned flags);

	    virtual void	show_short_help() const;
	    virtual int		max_insn_len() const;
	    virtual ColorAttr	get_insn_color(unsigned long clone);

	    virtual Bin_Format::bitness	get_bitness() const;
	    virtual char	clone_short_name(unsigned long clone);
	    virtual void	read_ini(Ini_Profile&);
	    virtual void	save_ini(Ini_Profile&);
	private:
	    BeyeContext&	bctx;
	    DisMode&		parent;
	    binary_stream&	main_handle;
	    const Bin_Format&	bin_format;
            char                *outstr;  
    };
}

#endif
