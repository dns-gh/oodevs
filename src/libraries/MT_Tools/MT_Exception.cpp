// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MT_Tools_pch.h"
#include "MT_Exception.h"
#include <sstream>

//-----------------------------------------------------------------------------
// Name: MT_Exception constructor
// Created: CBX 2002-07-31
//-----------------------------------------------------------------------------
MT_Exception::MT_Exception( const std::string& strContext, int nCode, const std::string& strInfo )
    : strContext_( strContext )
    , nCode_     ( nCode )
    , strInfo_   ( strInfo )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MT_Exception destructor
// Created:  NLD 00-02-22 
//-----------------------------------------------------------------------------
MT_Exception::~MT_Exception() throw()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MT_Exception::GetCode
// Created:  NLD 00-02-22 
//-----------------------------------------------------------------------------
int  MT_Exception::GetCode() const
{
    return nCode_;
}

//-----------------------------------------------------------------------------
// Name: std::string
// Created: CBX 2002-12-06
//-----------------------------------------------------------------------------
const std::string& MT_Exception::GetInfo() const
{
    return strInfo_;
}

//-----------------------------------------------------------------------------
// Name: std::string
// Created: CBX 2002-12-06
//-----------------------------------------------------------------------------
const std::string& MT_Exception::GetContext() const
{
    return strContext_;
}

//-----------------------------------------------------------------------------
// Name: MT_Exception::what
// Created: AGN 02-04-23
//-----------------------------------------------------------------------------
const char* MT_Exception::what() const throw()
{
    static std::string strWholeMessage;
    std::stringstream out;
    out << "[MT_Exception]" << std::endl
        << "\tContext: " << strContext_ << std::endl
        << "\tMessage: " << strInfo_ << std::endl
        << "\tCode:    " << nCode_ << std::endl;
    strWholeMessage = out.str();
    return strWholeMessage.c_str();
}
