// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-12-06 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Population_GUI.cpp $
// $Author: Ape $
// $Modtime: 7/06/05 10:08 $
// $Revision: 18 $
// $Workfile: ADN_Population_GUI.cpp $
//
// *****************************************************************************

#include "ADN_pch.h"
#include "ADN_Population_GUI.h"

#include <qlabel.h>
#include <qlayout.h>
#include <qgroupbox.h>
#include <qdialog.h>

#include "ADN_MainWindow.h"
#include "ADN_App.h"
#include "ADN_GuiBuilder.h"
#include "ADN_Workspace.h"
#include "ADN_CommonGfx.h"
#include "ADN_Population_Data.h"
#include "ADN_Connector_Vector_ABC.h"
#include "ADN_Population_ListView.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_GroupBox.h"
#include "ADN_Tr.h"

// -----------------------------------------------------------------------------
// Name: ADN_Population_GUI constructor
// Created: APE 2004-12-10
// -----------------------------------------------------------------------------
ADN_Population_GUI::ADN_Population_GUI( ADN_Population_Data& data )
: ADN_GUI_ABC( "ADN_Population_GUI" )
, data_      ( data )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Population_GUI destructor
// Created: APE 2004-12-10
// -----------------------------------------------------------------------------
ADN_Population_GUI::~ADN_Population_GUI()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Population_GUI::Build
// Created: APE 2004-12-10
// -----------------------------------------------------------------------------
void ADN_Population_GUI::Build()
{
    assert( pMainWidget_ == 0 );

    ADN_GuiBuilder builder;

    // Create the top widget.
    pMainWidget_ = new QWidget( 0, "Population main widget" );

    // Create the automata listview.
    ADN_Population_ListView* pPopulationList = new ADN_Population_ListView( pMainWidget_ );
    pPopulationList->GetConnector().Connect( &data_.GetPopulation() );
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, (ADN_Connector_ABC*)0 );

    QGroupBox* pGroup = new QGroupBox( 0, Qt::Horizontal, tr( "Population" ), pMainWidget_ );

    QWidget* pPropertiesGroup = builder.AddFieldHolder( pGroup );

    // Name
    builder.AddField<ADN_EditLine_String>( pPropertiesGroup, tr( "Name" ), vInfosConnectors[eName] );

    // Model
    builder.AddField< ADN_ComboBox_Vector<ADN_Models_Data::ModelInfos> >( pPropertiesGroup, tr( "Model" ), vInfosConnectors[eModel] );
    
    // Concentration density
    builder.AddField<ADN_EditLine_Double>( pPropertiesGroup, tr( "Concentration density" ), vInfosConnectors[eConcentrationDensity] );

    // Move nominal density
    builder.AddField<ADN_EditLine_Double>( pPropertiesGroup, tr( "Move nominal density" ), vInfosConnectors[eMoveDensity] );

    // Move speed
    builder.AddField<ADN_EditLine_Double>( pPropertiesGroup, tr( "Move speed" ), vInfosConnectors[eMoveSpeed] );

    pPopulationList->SetItemConnectors( vInfosConnectors );

    // Layout
    QHBoxLayout* pMainLayout = new QHBoxLayout( pMainWidget_, 10, 10 );
    pMainLayout->addWidget( pPopulationList, 1 );
    pMainLayout->addWidget( pGroup, 3 );

    QVBoxLayout* pGroupLayout = new QVBoxLayout( pGroup->layout(), 5 );
    pGroupLayout->addWidget( pPropertiesGroup, 0, 0 );
    builder.AddStretcher( pGroupLayout, Qt::Vertical );
}
