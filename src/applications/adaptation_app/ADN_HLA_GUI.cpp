// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-22 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_HLA_GUI.cpp $
// $Author: Ape $
// $Modtime: 25/04/05 12:06 $
// $Revision: 5 $
// $Workfile: ADN_HLA_GUI.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_HLA_GUI.h"

#include "ADN_HLA_Data.h"
#include "ADN_GuiBuilder.h"
#include "ADN_EditLine.h"
#include "ADN_Workspace.h"
#include "ADN_Project_GUI.h"

#include <qlayout.h>
#include <qlabel.h>


// -----------------------------------------------------------------------------
// Name: ADN_HLA_GUI constructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_HLA_GUI::ADN_HLA_GUI( ADN_HLA_Data& data )
: ADN_GUI_ABC( "ADN_HLA_GUI" )
, data_      ( data )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_HLA_GUI destructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_HLA_GUI::~ADN_HLA_GUI()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_HLA_GUI::Build
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
void ADN_HLA_GUI::Build()
{
    assert( pMainWidget_ == 0 );
    ADN_GuiBuilder builder;

    // Create the top widget.
    pMainWidget_ = new QWidget( 0 );
    QGroupBox* pGroup = new QGroupBox( 3, Qt::Horizontal, tr( "HLA" ), pMainWidget_ );

    builder.AddField<ADN_CheckBox>( pGroup, tr( "Use HLA" ), data_.bUseHLA_ );
    builder.AddField<ADN_EditLine_String>( pGroup, tr( "Federation" ), data_.strFederation_ );
    builder.AddField<ADN_EditLine_String>( pGroup, tr( "Federate" ), data_.strFederate_ );

    // Layout
    QVBoxLayout* pLayout = new QVBoxLayout( pMainWidget_, 0, 5 );
    pLayout->setAlignment( Qt::AlignTop );
    pLayout->addWidget( pGroup );

    // Ugly, but easier that way.
    ADN_Workspace::GetWorkspace().GetProject().GetGui().AddHLAWidget( *pMainWidget_ );
}
