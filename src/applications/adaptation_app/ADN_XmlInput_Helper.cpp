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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_XmlInput_Helper.cpp $
// $Author: Ape $
// $Modtime: 21/04/05 18:28 $
// $Revision: 5 $
// $Workfile: ADN_XmlInput_Helper.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_XmlInput_Helper.h"

#include "ADN_Xml_Exception.h"

// -----------------------------------------------------------------------------
// Name: ADN_XmlInput_Helper constructor
// Created: AGN 2004-07-01
// -----------------------------------------------------------------------------
ADN_XmlInput_Helper::ADN_XmlInput_Helper()
{
}

// -----------------------------------------------------------------------------
// Name: ADN_XmlInput_Helper destructor
// Created: AGN 2004-07-01
// -----------------------------------------------------------------------------
ADN_XmlInput_Helper::~ADN_XmlInput_Helper()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_XmlInput_Helper::ThrowError
// Created: AGN 2004-07-01
// -----------------------------------------------------------------------------
void ADN_XmlInput_Helper::ThrowError( const std::string& strMessage ) const
{
    throw ADN_Xml_Exception( GetContext(), strMessage );
}
