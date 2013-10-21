// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Nature_GUI.h"
#include "moc_ADN_Nature_GUI.cpp"
#include "ADN_Connector_String.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ADN_Nature_GUI constructor
// Created: ABR 2011-11-02
// -----------------------------------------------------------------------------
ADN_Nature_GUI::ADN_Nature_GUI( QGridLayout* parent, int row /* = 0 */ )
    : NatureEditionWidget( parent, row )
    , ADN_Gfx_ABC()
{
    pConnector_.reset( new ADN_Connector_String< ADN_Nature_GUI >( this ) );
    connect( this, SIGNAL( textChanged( const QString& ) ), SLOT( OnTextChanged( const QString& ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Nature_GUI destructor
// Created: AGE 2006-10-24
// -----------------------------------------------------------------------------
ADN_Nature_GUI::~ADN_Nature_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Nature_GUI::OnTextChanged
// Created: SBO 2010-02-15
// -----------------------------------------------------------------------------
void ADN_Nature_GUI::OnTextChanged( const QString& value )
{
    static_cast< ADN_Connector_String< ADN_Nature_GUI >& >( *pConnector_ ).SetDataChanged( value );
}

// -----------------------------------------------------------------------------
// Name: ADN_Nature_GUI::OnUnitSymbolChanged
// Created: MMC 2011-07-07
// -----------------------------------------------------------------------------
void ADN_Nature_GUI::OnUnitSymbolChanged( const QString& nature )
{
    if( text() != nature )
        setText( nature );
}

// -----------------------------------------------------------------------------
// Name: ADN_Nature_GUI::Warn
// Created: ABR 2013-01-16
// -----------------------------------------------------------------------------
void ADN_Nature_GUI::Warn( ADN_ErrorStatus, const QString& )
{
    // NOTHING
}
