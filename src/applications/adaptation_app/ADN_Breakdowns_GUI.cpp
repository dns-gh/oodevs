// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-17 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_Breakdowns_GUI.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 10:08 $
// $Revision: 8 $
// $Workfile: ADN_Breakdowns_GUI.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Breakdowns_GUI.h"

#include "ADN_GuiBuilder.h"
#include "ADN_Breakdowns_Data.h"
#include "ADN_Breakdowns_ListView.h"
#include "ADN_Breakdowns_PartsTable.h"
#include "ADN_EditLine.h"
#include "ADN_TimeField.h"
#include "ADN_Tr.h"

#include <QtGui/qwidget.h>
#include <QtGui/qlayout.h>
#include <Qt3Support/q3groupbox.h>
#include <Qt3Support/q3vbox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qtabwidget.h>

// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_GUI constructor
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
ADN_Breakdowns_GUI::ADN_Breakdowns_GUI( ADN_Breakdowns_Data& data )
: ADN_GUI_ABC( "ADN_Breakdowns_GUI" )
, data_      ( data )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_GUI destructor
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
ADN_Breakdowns_GUI::~ADN_Breakdowns_GUI()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_GUI::Build
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
void ADN_Breakdowns_GUI::Build()
{
    assert( pMainWidget_ == 0 );
    ADN_GuiBuilder builder;

    // Create the main widget.
    pMainWidget_ = new QWidget( 0, "breakdowns main widget" );

    // Breakdown list
    T_ConnectorVector vInfosConnectors( eNbrBreakdownGuiElements, (ADN_Connector_ABC*)0 );
    ADN_Breakdowns_ListView* pBreakdownsListView = new ADN_Breakdowns_ListView( pMainWidget_ );
    pBreakdownsListView->GetConnector().Connect( &data_.vBreakdowns_ );

    // Breakdown parameters
    Q3GroupBox* pBreakdownGroup = new Q3GroupBox( 1, Qt::Horizontal, tr( "Breakdown" ), pMainWidget_ );

    QWidget* pHolder = builder.AddFieldHolder( pBreakdownGroup );

    builder.AddField<ADN_EditLine_String>( pHolder, tr( "Name" ), vInfosConnectors[eName] );
    builder.AddEnumField<E_BreakdownType>( pHolder, tr( "Type" ), vInfosConnectors[eType], ADN_Tr::ConvertFromBreakdownType );
    builder.AddEnumField<E_BreakdownNTI>( pHolder, tr( "Seriousness" ), vInfosConnectors[eNTI], ADN_Tr::ConvertFromBreakdownNTI );
    builder.AddField<ADN_TimeField>( pHolder, tr( "Repair duration" ), vInfosConnectors[eRepairTime] );
    builder.AddField<ADN_TimeField>( pHolder, tr( "Repair duration variance" ), vInfosConnectors[eRepairTimeVariance] );

    Q3GroupBox* pPartsGroup = new Q3GroupBox( 1, Qt::Horizontal, tr( "Required parts" ), pBreakdownGroup );
    ADN_Breakdowns_PartsTable* pPartsTable = new ADN_Breakdowns_PartsTable( pPartsGroup );
    vInfosConnectors[eParts] = & pPartsTable->GetConnector();

    builder.AddStretcher( pBreakdownGroup, Qt::Vertical );

    pBreakdownsListView->SetItemConnectors( vInfosConnectors );

    Q3GroupBox* pGeneralGroup = new Q3GroupBox( 1, Qt::Horizontal, tr( "General parameter" ), pMainWidget_ );
    QWidget* pHolder2 = builder.AddFieldHolder( pGeneralGroup );
    builder.AddField<ADN_TimeField>( pHolder2, tr( "Average diagnostic duration" ), data_.strAverageDiagnosticTime_ );

    // Layout
    Q3HBoxLayout* pLayout = new Q3HBoxLayout( pMainWidget_, 10, 10 );
    pLayout->addWidget( pBreakdownsListView, 1 );
    pLayout->addWidget( pBreakdownGroup, 3 );
    pLayout->addWidget( pGeneralGroup, 1 );
}
