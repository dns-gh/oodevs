// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-12-06 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_GUI_ABC.cpp $
// $Author: Ape $
// $Modtime: 11/04/05 18:29 $
// $Revision: 3 $
// $Workfile: ADN_GUI_ABC.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_GUI_ABC.h"

// -----------------------------------------------------------------------------
// Name: ADN_GUI_ABC::tr
// Created: APE 2005-04-11
// -----------------------------------------------------------------------------
QString ADN_GUI_ABC::tr( const char* s, const char* c )
{
    if( qApp )
        return qApp->translate( strClassName_, s, c, QApplication::DefaultCodec );
    else
        return QString::fromLatin1( s );
}

