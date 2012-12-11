// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "MT_Exception.h"
#include "MT_Tools/MT_Logger.h"

//-----------------------------------------------------------------------------
// Name: MT_Exception constructor
// Created: JVT 03-02-04
//-----------------------------------------------------------------------------
MT_Exception::MT_Exception( const std::string& file, const std::string& function, unsigned int line, const std::string& what )
    : tools::Exception( file, function, line, what )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MT_Exception destructor
// Created: JVT 03-02-04
//-----------------------------------------------------------------------------
MT_Exception::~MT_Exception()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MT_Exception::SendToLogger
// Created: JVT 03-02-04
//-----------------------------------------------------------------------------
void MT_Exception::SendToLogger() const
{
    std::stringstream strMsg;
    strMsg << "SWORD Exception - Context " << GetFunction() << " - File " << GetFile() << " - Line " << GetLine() << " - Message '" << what() << "' ";
    MT_LOG_ERROR_MSG( strMsg.str().c_str() );
}
