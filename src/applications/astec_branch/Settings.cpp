// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-06-01 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/Settings.cpp $
// $Author: Ape $
// $Modtime: 2/09/04 11:19 $
// $Revision: 2 $
// $Workfile: Settings.cpp $
//
// *****************************************************************************

#include "astec_pch.h"
#include "Settings.h"

#include <qmainwindow.h>
#include <qdockwindow.h>

// -----------------------------------------------------------------------------
// Name: Settings constructor
// Created: APE 2004-06-01
// -----------------------------------------------------------------------------
Settings::Settings()
    : QSettings()
{
}


// -----------------------------------------------------------------------------
// Name: Settings constructor
/** @param  nFormat 
*/
// Created: APE 2004-06-01
// -----------------------------------------------------------------------------
Settings::Settings( QSettings::Format nFormat )
    : QSettings( nFormat )
{
}


// -----------------------------------------------------------------------------
// Name: Settings destructor
// Created: APE 2004-06-01
// -----------------------------------------------------------------------------
Settings::~Settings()
{
}


// -----------------------------------------------------------------------------
// Name: Settings::WriteEntry
// Created: APE 2004-06-01
// -----------------------------------------------------------------------------
void Settings::WriteEntry( const QString& field, QWidget& widget )
{
    beginGroup( field );
    writeEntry( "/width",  widget.width() );
    writeEntry( "/height", widget.height() );
    writeEntry( "/x", widget.x() );
    writeEntry( "/y", widget.y() );
    writeEntry( "/visible", widget.isVisible() );
    endGroup();
}


// -----------------------------------------------------------------------------
// Name: Settings::ReadEntry
// Created: APE 2004-06-01
// -----------------------------------------------------------------------------
void Settings::ReadEntry( const QString& field, QWidget& widget, int nW, int nH, int nX, int nY, bool bVisible )
{
    beginGroup( field );
    widget.resize( readNumEntry( "/width", nW ), readNumEntry( "/height", nH ) );
    widget.move( readNumEntry( "/x", nX ), readNumEntry( "/y", nY ) );
    if( readBoolEntry( "/visible", bVisible ) )
        widget.show();
    else
        widget.hide();
    endGroup();
}
