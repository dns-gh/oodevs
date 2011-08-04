// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2003-12-09 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_OpenFile_Exception.cpp $
// $Author: Ape $
// $Modtime: 21/03/05 18:35 $
// $Revision: 4 $
// $Workfile: ADN_OpenFile_Exception.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_OpenFile_Exception.h"

// -----------------------------------------------------------------------------
// Name: ADN_OpenFile_Exception constructor
// Created: AGN 2003-12-09
// -----------------------------------------------------------------------------
ADN_OpenFile_Exception::ADN_OpenFile_Exception( const std::string& strFileName )
: strFileName_( strFileName )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_OpenFile_Exception constructor
// Created: AGN 2003-12-09
// -----------------------------------------------------------------------------
ADN_OpenFile_Exception::ADN_OpenFile_Exception( const ADN_OpenFile_Exception& rhs )
: strFileName_( rhs.strFileName_ )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_OpenFile_Exception destructor
// Created: AGN 2003-12-09
// -----------------------------------------------------------------------------
ADN_OpenFile_Exception::~ADN_OpenFile_Exception()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_OpenFile_Exception::GetExceptionTitle
// Created: APE 2005-03-15
// -----------------------------------------------------------------------------
std::string ADN_OpenFile_Exception::GetExceptionTitle() const
{
    return qApp->translate( "ADN_OpenFile_Exception", "File error" ).ascii();
}


// -----------------------------------------------------------------------------
// Name: ADN_OpenFile_Exception::GetExceptionMessage
// Created: APE 2005-03-15
// -----------------------------------------------------------------------------
std::string ADN_OpenFile_Exception::GetExceptionMessage() const
{
    return qApp->translate( "ADN_OpenFile_Exception", "Could not open file '%1'.\nMake sure the file exists." ).arg( strFileName_.c_str() ).ascii();
}



