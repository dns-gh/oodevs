// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-18 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_KnowledgeGroups_GUI.cpp $
// $Author: Ape $
// $Modtime: 13/04/05 17:56 $
// $Revision: 4 $
// $Workfile: ADN_KnowledgeGroups_GUI.cpp $
//
// *****************************************************************************

#include "ADN_pch.h"
#include "ADN_KnowledgeGroups_GUI.h"

#include "ADN_KnowledgeGroups_Data.h"
#include "ADN_KnowledgeGroups_ListView.h"
#include "ADN_GuiBuilder.h"
#include "ADN_EditLine.h"
#include "ADN_TimeField.h"

#include <qgroupbox.h>
#include <qhbox.h>
#include <qgrid.h>
#include <qlabel.h>
#include <qlayout.h>


// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_GUI constructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_KnowledgeGroups_GUI::ADN_KnowledgeGroups_GUI( ADN_KnowledgeGroups_Data& data )
: ADN_GUI_ABC( "ADN_KnowledgeGroups_GUI" )
, data_      ( data )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_GUI destructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_KnowledgeGroups_GUI::~ADN_KnowledgeGroups_GUI()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_GUI::Build
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
void ADN_KnowledgeGroups_GUI::Build()
{
    if( pMainWidget_ != 0 )
        return;

    ADN_GuiBuilder builder;

    // Create the top widget.
    pMainWidget_ = new QWidget( 0 );

    // Create the listview.
    ADN_KnowledgeGroups_ListView* pGroupsList = new ADN_KnowledgeGroups_ListView( pMainWidget_ );
    pGroupsList->GetConnector().Connect( &data_.vGroups_ );
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, (ADN_Connector_ABC*)0 );

    QGroupBox* pGroup = new QGroupBox( 3, Qt::Horizontal, tr( "Knowledge group" ), pMainWidget_ );

    builder.AddField<ADN_EditLine_String>( pGroup, tr( "Name" ), vInfosConnectors[eName] );
    builder.AddField<ADN_TimeField>( pGroup, tr( "Maximum lifetime" ), vInfosConnectors[eMaxLifetime] );
    builder.AddField<ADN_EditLine_Double>( pGroup, tr( "Maximum distance between knowledge and real unit" ), vInfosConnectors[eMaxDistance], 0, eGreaterZero );
    builder.AddOptionnalField<ADN_TimeField>( pGroup, tr( "Interpolation time" ), vInfosConnectors[eHasInterpolationTime], vInfosConnectors[eInterpolationTime] );

    pGroupsList->SetItemConnectors( vInfosConnectors );

    // Layout
    QHBoxLayout* pMainLayout = new QHBoxLayout( pMainWidget_, 10, 10 );
    pMainLayout->addWidget( pGroupsList, 1 );
    pMainLayout->addWidget( pGroup, 3 );
}
