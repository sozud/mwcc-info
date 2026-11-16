/************************************************************************/
/*	Project...:	Standard C++ Library									*/
/*	Name......:	stdexcept.c												*/
/*	Purpose...:	exception handling										*/
/*  Copyright.: ©Copyright 1993-95 by metrowerks inc					*/
/************************************************************************/

#define __NOSTRING__


#include <ansi_parms.h>             // hh 971207 <ansi_parms.h> must be first
#include <stdexcept>
#include <exception.h>
#include <typeinfo>

#ifdef __MODENALIB__

#if __MC68K__
#pragma far_code
#endif
/*
extern void __priv_throwbadcast(void)
{
	throw bad_cast("bad dynamic_cast");
}
*/
#else
/*
const char* exception::what() const throw() { return "exception"; }
const char* bad_cast::what() const throw() { return "bad_cast"; }
const char* bad_typeid::what() const throw() { return "bad_typeid"; }
const char* bad_exception::what() const throw() { return "bad_exception"; }

//const char* logic_error::what() const { return mwhat; }
const char* logic_error::what() const { return "logic_error"; }
const char* domain_error::what() const { return logic_error::what(); }
const char* invalid_argument::what() const { return logic_error::what(); }
const char* length_error::what() const { return logic_error::what(); }
const char* out_of_range::what() const { return logic_error::what(); }

//const char* runtime_error::what() const { return mwhat; }
const char* runtime_error::what() const { return "runtime_error"; }
const char* range_error::what() const { return runtime_error::what(); }
const char* overflow_error::what() const { return runtime_error::what(); }
*/
extern void __priv_throwbadcast(void)
{
	throw bad_cast();
}

#endif

// hh 971207 <ansi_parms.h> must be first
// hh 980205 stdexcept implementation moved to <stdexcept>
