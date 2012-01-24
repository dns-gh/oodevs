// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-21 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Communications_GUI.cpp $
// $Author: Ape $
// $Modtime: 13/04/05 17:48 $
// $Revision: 4 $
// $Workfile: ADN_Communications_GUI.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Communications_GUI.h"

#include "ADN_Communications_Data.h"
#include "ADN_GuiBuilder.h"
#include "ADN_EditLine.h"

// -----------------------------------------------------------------------------
// Name: ADN_Communications_GUI constructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Communications_GUI::ADN_Communications_GUI( ADN_Communications_Data& data )
    : ADN_GUI_ABC( "ADN_Communications_GUI" )
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
    ADN_GuiBuilder builder;

    // Info holders
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );
    builder.AddField<ADN_EditLine_Double>( pInfoHolder, tr( "Effect on reloading duration" ), data_.rReloadModifier_, 0, eGreaterZero );
    builder.AddField<ADN_EditLine_Double>( pInfoHolder, tr( "Effect on movement speed" ), data_.rSpeedModifier_, 0, eGreaterEqualZero );
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
    pMainWidget_ = CreateScrollArea( *pContent );
}
