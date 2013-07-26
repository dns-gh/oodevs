// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Communications_GUI.h"
#include "moc_ADN_Communications_GUI.cpp"
#include "ADN_Communications_Data.h"
#include "ADN_GuiBuilder.h"
#include "ADN_EditLine.h"

// -----------------------------------------------------------------------------
// Name: ADN_Communications_GUI constructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Communications_GUI::ADN_Communications_GUI( ADN_Communications_Data& data )
    : ADN_GUI_ABC( eCommunications )
    , data_      ( data )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Communications_GUI destructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Communications_GUI::~ADN_Communications_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Communications_GUI::Build
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
void ADN_Communications_GUI::Build()
{
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    assert( pMainWidget_ == 0 );
    ADN_GuiBuilder builder( strClassName_ );

    // Info holders
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );
    builder.AddField< ADN_EditLine_Double >( pInfoHolder, "effect-on-reloading-duration", tr( "Effect on reloading duration" ), data_.rReloadModifier_, 0, eGreaterZero );
    builder.AddField< ADN_EditLine_Double >( pInfoHolder, "effect-on-movement-speed", tr( "Effect on movement speed" ), data_.rSpeedModifier_, 0, eGreaterEqualZero );
    builder.AddStretcher( pInfoHolder, Qt::Vertical );

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // Content layout
    QWidget* pContent = new QWidget();
    QHBoxLayout* pContentLayout = new QHBoxLayout( pContent );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addWidget( pInfoHolder );

    // Main widget
    pMainWidget_ = CreateScrollArea( builder.GetName(), *pContent );
}
