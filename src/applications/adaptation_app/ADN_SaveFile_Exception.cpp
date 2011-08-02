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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_SaveFile_Exception.cpp $
// $Author: Ape $
// $Modtime: 21/03/05 18:35 $
// $Revision: 2 $
// $Workfile: ADN_SaveFile_Exception.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_SaveFile_Exception.h"

#include <QtGui/qapplication.h>


// -----------------------------------------------------------------------------
// Name: ADN_SaveFile_Exception constructor
// Created: AGN 2003-12-09
// -----------------------------------------------------------------------------
ADN_SaveFile_Exception::ADN_SaveFile_Exception( const std::string& strFileName )
: strFileName_( strFileName )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_SaveFile_Exception constructor
// Created: AGN 2003-12-09
// -----------------------------------------------------------------------------
ADN_SaveFile_Exception::ADN_SaveFile_Exception( const ADN_SaveFile_Exception& rhs )
: strFileName_( rhs.strFileName_ )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_SaveFile_Exception destructor
// Created: AGN 2003-12-09
// -----------------------------------------------------------------------------
ADN_SaveFile_Exception::~ADN_SaveFile_Exception()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_SaveFile_Exception::GetExceptionTitle
// Created: APE 2005-03-15
// -----------------------------------------------------------------------------
std::string ADN_SaveFile_Exception::GetExceptionTitle() const
{
    return qApp->translate( "ADN_SaveFile_Exception", "File error" ).ascii();
}


// -----------------------------------------------------------------------------
// Name: ADN_SaveFile_Exception::GetExceptionMessage
// Created: APE 2005-03-15
// -----------------------------------------------------------------------------
std::string ADN_SaveFile_Exception::GetExceptionMessage() const
{
    return qApp->translate( "ADN_SaveFile_Exception", "Could not save file '%1'.\nMake sure that the file is not write-protected." ).arg( strFileName_.c_str() ).ascii();
}



