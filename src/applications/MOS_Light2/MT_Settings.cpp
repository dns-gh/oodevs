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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MT_Settings.cpp $
// $Author: Ape $
// $Modtime: 2/09/04 11:19 $
// $Revision: 2 $
// $Workfile: MT_Settings.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_Pch.h"
#include "MT_Settings.h"

#include <qmainwindow.h>
#include <qdockwindow.h>

#ifndef MT_USE_INLINE
#   include "MT_Settings.inl"
#endif


// -----------------------------------------------------------------------------
// Name: MT_Settings constructor
// Created: APE 2004-06-01
// -----------------------------------------------------------------------------
MT_Settings::MT_Settings()
    : QSettings()
{
}


// -----------------------------------------------------------------------------
// Name: MT_Settings constructor
/** @param  nFormat 
*/
// Created: APE 2004-06-01
// -----------------------------------------------------------------------------
MT_Settings::MT_Settings( QSettings::Format nFormat )
    : QSettings( nFormat )
{
}


// -----------------------------------------------------------------------------
// Name: MT_Settings destructor
// Created: APE 2004-06-01
// -----------------------------------------------------------------------------
MT_Settings::~MT_Settings()
{
}


// -----------------------------------------------------------------------------
// Name: MT_Settings::WriteEntry
/** @param  field 
    @param  widget 
*/
// Created: APE 2004-06-01
// -----------------------------------------------------------------------------
void MT_Settings::WriteEntry( const QString& field, QWidget& widget )
{
    this->beginGroup( field );
    this->writeEntry( "/width",  widget.width() );
    this->writeEntry( "/height", widget.height() );
    this->writeEntry( "/x", widget.x() );
    this->writeEntry( "/y", widget.y() );
    this->writeEntry( "/visible", widget.isVisible() );
    this->endGroup();
}


// -----------------------------------------------------------------------------
// Name: MT_Settings::ReadEntry
/** @param  field 
    @param  widget 
*/
// Created: APE 2004-06-01
// -----------------------------------------------------------------------------
void MT_Settings::ReadEntry( const QString& field, QWidget& widget, int nW, int nH, int nX, int nY, bool bVisible )
{
    this->beginGroup( field );
    widget.resize( readNumEntry( "/width", nW ), readNumEntry( "/height", nH ) );
    widget.move( readNumEntry( "/x", nX ), readNumEntry( "/y", nY ) );
    if( readBoolEntry( "/visible", bVisible ) )
        widget.show();
    else
        widget.hide();
    this->endGroup();
}
