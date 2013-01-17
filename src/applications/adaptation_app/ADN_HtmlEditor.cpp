// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_HtmlEditor.h"
#include "moc_ADN_HtmlEditor.cpp"
#include "ADN_Connector_String.h"

// -----------------------------------------------------------------------------
// Name: ADN_HtmlEditor constructor
// Created: NPT 2012-07-27
// -----------------------------------------------------------------------------
ADN_HtmlEditor::ADN_HtmlEditor( int optionFlags /* = eAllMask */ )
    : HtmlEditor( 0, optionFlags )
{
    pConnector_ = new ADN_Connector_String< ADN_HtmlEditor >( this );
    connect( this, SIGNAL( TextChanged() ), this, SLOT( OnTextChanged() ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_HtmlEditor destructor
// Created: NPT 2012-07-27
// -----------------------------------------------------------------------------
ADN_HtmlEditor::~ADN_HtmlEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_HtmlEditor::text
// Created: NPT 2012-07-27
// -----------------------------------------------------------------------------
QString ADN_HtmlEditor::text() const
{
    return GetText();
}

// -----------------------------------------------------------------------------
// Name: ADN_HtmlEditor::setText
// Created: NPT 2012-07-27
// -----------------------------------------------------------------------------
void ADN_HtmlEditor::setText( const QString text )
{
    SetText( text );
}

// -----------------------------------------------------------------------------
// Name: ADN_HtmlEditor::OnTextChanged
// Created: NPT 2012-07-30
// -----------------------------------------------------------------------------
void ADN_HtmlEditor::OnTextChanged()
{
    static_cast< ADN_Connector_String< ADN_HtmlEditor >* >( pConnector_ )->SetDataChanged( GetText() );
}

// -----------------------------------------------------------------------------
// Name: ADN_HtmlEditor::Warn
// Created: ABR 2013-01-16
// -----------------------------------------------------------------------------
void ADN_HtmlEditor::Warn( ADN_ErrorStatus, const QString& )
{
    // NOTHING
}
