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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_DataException.cpp $
// $Author: Ape $
// $Modtime: 21/03/05 18:35 $
// $Revision: 4 $
// $Workfile: ADN_DataException.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_DataException.h"

#include <QtGui/qapplication.h>

// -----------------------------------------------------------------------------
// Name: ADN_DataException constructor
// Created: AGN 2003-12-09
// -----------------------------------------------------------------------------
ADN_DataException::ADN_DataException( const std::string& strErrorTitle, const std::string& strErrorDescription, const std::string& strErrorSolution )
: strErrorTitle_( strErrorTitle )
, strErrorDescription_( strErrorDescription )
, strErrorSolution_( strErrorSolution )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_DataException constructor
// Created: AGN 2003-12-09
// -----------------------------------------------------------------------------
ADN_DataException::ADN_DataException( const ADN_DataException& rhs )
: strErrorTitle_( rhs.strErrorTitle_ )
, strErrorDescription_( rhs.strErrorDescription_ )
, strErrorSolution_( rhs.strErrorSolution_ )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_DataException destructor
// Created: AGN 2003-12-09
// -----------------------------------------------------------------------------
ADN_DataException::~ADN_DataException()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_DataException::GetExceptionTitle
// Created: APE 2005-03-15
// -----------------------------------------------------------------------------
std::string ADN_DataException::GetExceptionTitle() const
{
    return qApp->translate( "ADN_DataException", "Data error" ).ascii();
}


// -----------------------------------------------------------------------------
// Name: ADN_DataException::GetExceptionMessage
// Created: APE 2005-03-15
// -----------------------------------------------------------------------------
std::string ADN_DataException::GetExceptionMessage() const
{
    std::string strMsg = strErrorDescription_;
    strMsg += strErrorSolution_;
    return strMsg;
}
