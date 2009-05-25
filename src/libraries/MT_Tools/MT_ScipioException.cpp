//*****************************************************************************
//
// $Created: JVT 03-02-04 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_ScipioException.cpp $
// $Author: Age $
// $Modtime: 14/04/05 10:35 $
// $Revision: 3 $
// $Workfile: MT_ScipioException.cpp $
//
//*****************************************************************************

#include "MT_Tools_pch.h"

#include "MT_ScipioException.h"

#include "MT/MT_Logger/MT_Logger_lib.h"


//-----------------------------------------------------------------------------
// Name: MT_ScipioException constructor
// Created: JVT 03-02-04
//-----------------------------------------------------------------------------
MT_ScipioException::MT_ScipioException( const std::string& strContext, const std::string& strFile, uint nLine, const std::string& strMessage, const std::string& strDescription )
    : MT_Exception( strContext, 42, strMessage )
    , strContext_    ( strContext )
    , strFile_       ( strFile )
    , nLine_         ( nLine )
    , strMessage_    ( strMessage )
    , strDescription_( strDescription )
{    
}

//-----------------------------------------------------------------------------
// Name: MT_ScipioException constructor
// Created: JVT 03-02-04
//-----------------------------------------------------------------------------
MT_ScipioException::MT_ScipioException( const std::string& strContext, const std::string& strFile, uint nLine, const std::string& strMessage )
    : MT_Exception( strContext, 42, strMessage )
    , strContext_    ( strContext )
    , strFile_       ( strFile )
    , nLine_         ( nLine )
    , strMessage_    ( strMessage )
    , strDescription_()
{
}


//-----------------------------------------------------------------------------
// Name: MT_ScipioException constructor
// Created: JVT 03-02-04
//-----------------------------------------------------------------------------
MT_ScipioException::MT_ScipioException( const MT_ScipioException& rhs )
    : MT_Exception( rhs )
    , strContext_    ( rhs.strContext_ )
    , strFile_       ( rhs.strFile_ )
    , nLine_         ( rhs.nLine_ )
    , strMessage_    ( rhs.strMessage_ )
    , strDescription_( rhs.strDescription_ )
{
}


//-----------------------------------------------------------------------------
// Name: MT_ScipioException::operator =
// Created: JVT 03-02-04
//-----------------------------------------------------------------------------
MT_ScipioException& MT_ScipioException::operator = ( const MT_ScipioException& rhs )
{
    strContext_     = rhs.strContext_;
    strFile_        = rhs.strFile_;
    nLine_          = rhs.nLine_;
    strMessage_     = rhs.strMessage_;
    strDescription_ = rhs.strDescription_;

    return *this;
}



//-----------------------------------------------------------------------------
// Name: MT_ScipioException destructor
// Created: JVT 03-02-04
//-----------------------------------------------------------------------------
MT_ScipioException::~MT_ScipioException()
{
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
