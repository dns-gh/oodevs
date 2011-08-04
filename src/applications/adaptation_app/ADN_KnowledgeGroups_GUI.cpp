// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_KnowledgeGroups_GUI.h"
#include "ADN_KnowledgeGroups_Data.h"
#include "ADN_KnowledgeGroups_ListView.h"
#include "ADN_GuiBuilder.h"
#include "ADN_EditLine.h"
#include "ADN_TimeField.h"

// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_GUI constructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_KnowledgeGroups_GUI::ADN_KnowledgeGroups_GUI( ADN_KnowledgeGroups_Data& data )
    : ADN_GUI_ABC( "ADN_KnowledgeGroups_GUI" )
    , data_( data )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_GUI destructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_KnowledgeGroups_GUI::~ADN_KnowledgeGroups_GUI()
{
    // NOTHING
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

    Q3GroupBox* pGroup = new Q3GroupBox( 4, Qt::Vertical, tr( "Knowledge group" ), pMainWidget_ );

    QWidget* pHolder = builder.AddFieldHolder( pGroup );
    builder.AddField<ADN_EditLine_String>( pHolder, tr( "Name" ), vInfosConnectors[eName] );

    Q3GroupBox* pDelayGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Delay Parameters" ), pGroup );
    builder.AddField<ADN_TimeField>( pDelayGroup, tr( "Communication Delay" ), vInfosConnectors[eCommunicationDelay] );

    Q3GroupBox* pAgentGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Parameters on known units" ), pGroup );
    builder.AddField<ADN_TimeField>( pAgentGroup, tr( "Maximum life span" ), vInfosConnectors[eAgentMaxLifetime] )->SetMinimumValueInSecond( 1 );
    builder.AddField<ADN_EditLine_Double>( pAgentGroup, tr( "Maximum distance between known unit and real unit positions" ), vInfosConnectors[eAgentMaxDistance], 0, eGreaterZero );
    builder.AddOptionnalField<ADN_TimeField>( pAgentGroup, tr( "Extrapolation duration" ), vInfosConnectors[eAgentHasInterpolationTime], vInfosConnectors[eAgentInterpolationTime] );

    Q3GroupBox* pPopulationGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Parameters on known crowds" ), pGroup );
    builder.AddField<ADN_TimeField>( pPopulationGroup, tr( "Maximum life span" ), vInfosConnectors[ePopulationMaxLifetime] )->SetMinimumValueInSecond( 1 );

    pGroupsList->SetItemConnectors( vInfosConnectors );

    // Layout
    Q3HBoxLayout* pMainLayout = new Q3HBoxLayout( pMainWidget_, 10, 10 );
    pMainLayout->addWidget( pGroupsList, 1 );
    pMainLayout->addWidget( pGroup, 4 );
}
