/**
 * @namespace	usr_plugins_auto
 * @file        plugins/bin/mz.h
 * @brief       This file contains MZ executable file definitions.
 * @version     -
 * @remark      this source file is part of Binary EYE project (BEYE).
 *              The Binary EYE (BEYE) is copyright (C) 1995 Nickols_K.
 *              All rights reserved. This software is redistributable under the
 *              licence given in the file "Licence.en" ("Licence.ru" in russian
 *              translation) distributed in the BEYE archive.
 * @note        Requires POSIX compatible development system
 *
 * @author      Nickols_K
 * @since       1995
 * @note        Development, fixes and improvements
**/
#ifndef __MZ_INC
#define __MZ_INC

#include "config.h"
#include "beyeutil.h"
#include "plugins/binary_parser.h"

namespace	usr {
#ifdef __HAVE_PRAGMA_PACK__
#pragma pack(1)
#endif
    struct MZHEADER {
	uint16_t mzPartLastPage;
	uint16_t mzPageCount;
	uint16_t mzRelocationCount;
	uint16_t mzHeaderSize;
	uint16_t mzMinMem;
	uint16_t mzMaxMem;
	uint16_t mzRelocationSS;
	uint16_t mzExeSP;
	uint16_t mzCheckSumm;
	uint16_t mzExeIP;
	uint16_t mzRelocationCS;
	uint16_t mzTableOffset;
	uint16_t mzOverlayNumber;
    };
#ifdef __HAVE_PRAGMA_PACK__
#pragma pack()
#endif
    class MZ_Parser : public Binary_Parser {
	public:
	    MZ_Parser(binary_stream& h,CodeGuider&);
	    virtual ~MZ_Parser();

	    virtual const char*		prompt(unsigned idx) const;
	    virtual __filesize_t	action_F1();

	    virtual __filesize_t	show_header();
	    virtual bool		bind(const DisMode& _parent,std::string& str,__filesize_t shift,int flg,int codelen,__filesize_t r_shift);
	    virtual int			query_platform() const;
	    virtual bool		address_resolving(std::string&,__filesize_t);
	    virtual __filesize_t	va2pa(__filesize_t va);
	    virtual __filesize_t	pa2va(__filesize_t pa);

	    static __filesize_t		is_new_exe(binary_stream& main_handle);
	protected:
	    CodeGuider&			code_guider() const { return _code_guider; }
	    binary_stream&		main_handle() const { return _main_handle; }
	    virtual __filesize_t	headshift() const { return _headshift; }
	private:
	    const char*			QueryAddInfo( unsigned char *memmap );
	    const char*			QueryAddInfo();
	    void			BuildMZChain();
	    bool			isMZReferenced(__filesize_t shift,char len);

	    static tCompare		compare_ptr(const any_t*e1,const any_t*e2);
	    static tCompare		compare_mz(const any_t*e1,const any_t*e2);

	    MZHEADER		mz;
	    unsigned long	HeadSize;
	    long*		CurrMZChain;
	    unsigned long	CurrMZCount;
	    __filesize_t	_headshift;
	    binary_stream&	_main_handle;
	    CodeGuider&		_code_guider;
    };
} // namespace	usr
#endif
