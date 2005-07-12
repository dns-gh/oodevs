// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-07-01 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Xml_Exception.cpp $
// $Author: Ape $
// $Modtime: 21/03/05 18:35 $
// $Revision: 4 $
// $Workfile: ADN_Xml_Exception.cpp $
//
// *****************************************************************************

#include "ADN_pch.h"
#include "ADN_Xml_Exception.h"

#include <qapplication.h>


// -----------------------------------------------------------------------------
// Name: ADN_Xml_Exception constructor
// Created: AGN 2004-07-01
// -----------------------------------------------------------------------------
ADN_Xml_Exception::ADN_Xml_Exception( const std::string& strFile, const std::string& strContext, const std::string& strMessage )
: strFile_( strFile )
, strContext_( strContext )
, strMessage_( strMessage )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Xml_Exception constructor
// Created: AGN 2004-07-01
// -----------------------------------------------------------------------------
ADN_Xml_Exception::ADN_Xml_Exception( const std::string& strContext, const std::string& strMessage )
: strFile_()
, strContext_( strContext )
, strMessage_( strMessage )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Xml_Exception constructor
// Created: AGN 2004-07-01
// -----------------------------------------------------------------------------
ADN_Xml_Exception::ADN_Xml_Exception( const ADN_Xml_Exception& rhs )
: strFile_( rhs.strFile_ )
, strContext_( rhs.strContext_ )
, strMessage_( rhs.strMessage_ )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Xml_Exception destructor
// Created: AGN 2004-07-01
// -----------------------------------------------------------------------------
ADN_Xml_Exception::~ADN_Xml_Exception()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Xml_Exception::GetExceptionTitle
// Created: APE 2005-03-15
// -----------------------------------------------------------------------------
std::string ADN_Xml_Exception::GetExceptionTitle() const
{
    return qApp->translate( "ADN_Xml_Exception", "XML error" ).ascii();
}


// -----------------------------------------------------------------------------
// Name: ADN_Xml_Exception::GetExceptionMessage
// Created: APE 2005-03-15
// -----------------------------------------------------------------------------
std::string ADN_Xml_Exception::GetExceptionMessage() const
{
    QString strMsg;
    if( strFile_.empty() )
    {
        strMsg = qApp->translate( "ADN_Xml_Exception", "An error occured.\nContext:\n%1\nError: %2" );
        strMsg = strMsg.arg( strContext_.c_str(), strMessage_.c_str() );
    }
    else
    {
        strMsg = qApp->translate( "ADN_Xml_Exception", "An error occured in file '%1'.\nContext:\n%2\nError: %3" );
        strMsg = strMsg.arg( strFile_.c_str(), strContext_.c_str(), strMessage_.c_str() );
    }
    return strMsg.ascii();
}



