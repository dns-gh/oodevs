// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_People_GUI.h"
#include "ADN_MainWindow.h"
#include "ADN_App.h"
#include "ADN_GuiBuilder.h"
#include "ADN_Workspace.h"
#include "ADN_CommonGfx.h"
#include "ADN_People_Data.h"
#include "ADN_Connector_Vector_ABC.h"
#include "ADN_People_ListView.h"
#include "ADN_ComboBox_Vector.h"

// -----------------------------------------------------------------------------
// Name: ADN_People_GUI constructor
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
ADN_People_GUI::ADN_People_GUI( ADN_People_Data& data )
    : ADN_GUI_ABC( "ADN_People_GUI" )
    , data_( data )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: ADN_People_GUI destructor
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
ADN_People_GUI::~ADN_People_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_People_GUI::Build
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void ADN_People_GUI::Build()
{
    assert( pMainWidget_ == 0 );

    ADN_GuiBuilder builder;

    // Create the top widget.
    pMainWidget_ = new QWidget( 0, "Population main widget" );

    // Create the population listview.
    ADN_People_ListView* pPeopleList = new ADN_People_ListView( pMainWidget_ );
    pPeopleList->GetConnector().Connect( &data_.GetPeople() );
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, (ADN_Connector_ABC*)0 );

    QVBox* pMainBox = new QVBox( pMainWidget_ );

    // Population parameters
    QGroupBox* pGroup = new QGroupBox( 1, Qt::Horizontal, tr( "Population" ), pMainBox );

    QGroupBox* pPropertiesGroup = new QGroupBox( 3, Qt::Horizontal, tr( "Details" ), pGroup );

    builder.AddField< ADN_ComboBox_Vector<ADN_Population_Data::PopulationInfos> >( pPropertiesGroup, tr( "Associated Crowd" ), vInfosConnectors[eModel] );
    builder.AddField<ADN_EditLine_Int>( pPropertiesGroup, tr( "Male" ), vInfosConnectors[eMale], tr( "%" ), ePercentage );
    builder.AddField<ADN_EditLine_Int>( pPropertiesGroup, tr( "Female" ), vInfosConnectors[eFemale], tr( "%" ), ePercentage );
    builder.AddField<ADN_EditLine_Int>( pPropertiesGroup, tr( "Children" ), vInfosConnectors[eChildren], tr( "%" ), ePercentage );

    
    pPeopleList->SetItemConnectors( vInfosConnectors );

    // Layout
    QHBoxLayout* pMainLayout = new QHBoxLayout( pMainWidget_, 10, 10 );
    pMainLayout->addWidget( pPeopleList, 1 );
    pMainLayout->addWidget( pMainBox, 3 );
}
