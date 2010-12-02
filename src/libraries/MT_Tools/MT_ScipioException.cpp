// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MT_ScipioException.h"
#include "MT_Tools/MT_Logger.h"

//-----------------------------------------------------------------------------
// Name: MT_ScipioException constructor
// Created: JVT 03-02-04
//-----------------------------------------------------------------------------
MT_ScipioException::MT_ScipioException( const std::string& strContext, const std::string& strFile, unsigned int nLine, const std::string& strMessage, const std::string& strDescription )
    : strContext_    ( strContext )
    , strFile_       ( strFile )
    , nLine_         ( nLine )
    , strMessage_    ( strMessage )
    , strDescription_( strDescription )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MT_ScipioException constructor
// Created: JVT 03-02-04
//-----------------------------------------------------------------------------
MT_ScipioException::MT_ScipioException( const std::string& strContext, const std::string& strFile, unsigned int nLine, const std::string& strMessage )
    : strContext_( strContext )
    , strFile_   ( strFile )
    , nLine_     ( nLine )
    , strMessage_( strMessage )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MT_ScipioException destructor
// Created: JVT 03-02-04
//-----------------------------------------------------------------------------
MT_ScipioException::~MT_ScipioException()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MT_ScipioException::SendToLogger
// Created: JVT 03-02-04
//-----------------------------------------------------------------------------
void MT_ScipioException::SendToLogger() const
{
    std::stringstream strMsg;
    strMsg << "SWORD Exception - Context " << strContext_ << " - File " << strFile_ << " - Line " << nLine_ << " - Message '" << strMessage_ << "' ";
    if( !strDescription_.empty() )
        strMsg << strDescription_;
    MT_LOG_ERROR_MSG( strMsg.str().c_str() );
}
