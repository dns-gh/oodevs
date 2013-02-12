// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_KnowledgeGroups_GUI.h"
#include "moc_ADN_KnowledgeGroups_GUI.cpp"
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
    : ADN_GUI_ABC( eKnowledgeGroups )
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
    // -------------------------------------------------------------------------
    // Creation
    // -------------------------------------------------------------------------
    assert( pMainWidget_ == 0 );
    ADN_GuiBuilder builder( strClassName_ );
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    // Info holder
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );
    ADN_EditLine_ABC* nameField = builder.AddField< ADN_EditLine_String >( pInfoHolder, "name", tr( "Name" ), vInfosConnectors[eName] );
    nameField->ConnectWithRefValidity( data_.GetGroupInfos() );

    Q3GroupBox* pDelayGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Delay Parameters" ) );
    builder.AddField< ADN_TimeField >( pDelayGroup, "communication-delay", tr( "Communication Delay" ), vInfosConnectors[eCommunicationDelay] );

    Q3GroupBox* pAgentGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Parameters on known units" ) );
    builder.AddField< ADN_TimeField >( pAgentGroup, "maximum-life-span", tr( "Maximum life span" ), vInfosConnectors[eAgentMaxLifetime] )->SetMinimumValueInSecond( 1 );
    builder.AddField< ADN_EditLine_Double >( pAgentGroup, "maximum-distance-between-known-unit", tr( "Maximum distance between known unit and real unit positions" ), vInfosConnectors[eAgentMaxDistance], 0, eGreaterZero );
    builder.AddOptionnalField<ADN_TimeField>( pAgentGroup, "extrapolation-duration", tr( "Extrapolation duration" ), vInfosConnectors[eAgentHasInterpolationTime], vInfosConnectors[eAgentInterpolationTime] );

    Q3GroupBox* pPopulationGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Parameters on known crowds" ) );
    builder.AddField< ADN_TimeField >( pPopulationGroup, "maximum-life-span", tr( "Maximum life span" ), vInfosConnectors[ePopulationMaxLifetime] )->SetMinimumValueInSecond( 1 );

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // Content layout
    QWidget* pContent = new QWidget();
    QVBoxLayout* pContentLayout = new QVBoxLayout( pContent );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addWidget( pInfoHolder );
    pContentLayout->addWidget( pDelayGroup );
    pContentLayout->addWidget( pAgentGroup );
    pContentLayout->addWidget( pPopulationGroup );

    // List view
    QWidget* pSearchListView = builder.AddSearchListView< ADN_KnowledgeGroups_ListView >( this, data_.vGroups_, vInfosConnectors );

    // Main widget
    pMainWidget_ = CreateScrollArea( builder.GetName(), *pContent, pSearchListView );
}
