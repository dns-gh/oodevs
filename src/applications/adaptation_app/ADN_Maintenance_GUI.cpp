// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Maintenance_GUI.h"

#include "ADN_Maintenance_Data.h"
#include "ADN_GuiBuilder.h"
#include "ADN_EditLine.h"
#include "ADN_Table.h"
#include "ADN_TableItem_Edit.h"
#include "ADN_TableItem_TimeField.h"
#include "ADN_AvailabilityWarningTable.h"

#include "ENT/ENT_Tr.h"

#include <Qt3Support/q3groupbox.h>
#include <Qt3Support/q3hgroupbox.h>
#include <Qt3Support/q3hbox.h>
#include <Qt3Support/q3grid.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>


// -----------------------------------------------------------------------------
// Name: ADN_Maintenance_GUI constructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Maintenance_GUI::ADN_Maintenance_GUI( ADN_Maintenance_Data& data )
: ADN_GUI_ABC( "ADN_Maintenance_GUI" )
, data_      ( data )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Maintenance_GUI destructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Maintenance_GUI::~ADN_Maintenance_GUI()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Maintenance_GUI::Build
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
void ADN_Maintenance_GUI::Build()
{
    pMainWidget_ = new QWidget( 0 );

    Q3HGroupBox* pGroup = new Q3HGroupBox( tr( "Maintenance system data" ), pMainWidget_ );
    BuildWorkingSchemeTable( pGroup );

    Q3HGroupBox* pWarningGroup = new Q3HGroupBox( tr( "Equipments availability warnings" ), pGroup );
    Q3HGroupBox* pRepairerGroup = new Q3HGroupBox( tr( "Repairers" ), pWarningGroup );
    ADN_AvailabilityWarningTable* pRepairerWarningTable = new ADN_AvailabilityWarningTable( pRepairerGroup );
    pRepairerWarningTable->GetConnector().Connect( & data_.vRepairerWarnings_ );
    Q3HGroupBox* pHaulerGroup = new Q3HGroupBox( tr( "Tow trucks" ), pWarningGroup );
    ADN_AvailabilityWarningTable* pHaulerWarningTable = new ADN_AvailabilityWarningTable( pHaulerGroup );
    pHaulerWarningTable->GetConnector().Connect( & data_.vHaulerWarnings_ );

    // Layout
    Q3VBoxLayout* pLayout = new Q3VBoxLayout( pMainWidget_, 10, 10 );
    pLayout->addWidget( pGroup );
}

// -----------------------------------------------------------------------------
// Name: ADN_Maintenance_GUI::BuildWorkingSchemeTable
// Created: SBO 2006-08-04
// -----------------------------------------------------------------------------
void ADN_Maintenance_GUI::BuildWorkingSchemeTable( QWidget* parent )
{
    ADN_GuiBuilder builder;

    Q3GroupBox* pGroup = new Q3HGroupBox( tr( "Shifts durations" ), parent );

    ADN_Table* pTable = builder.CreateTable( pGroup );
    pTable->setNumCols( static_cast< int >( data_.vWorkingSchemes_.size() ) );
    pTable->setNumRows( 2 );
    pTable->verticalHeader()->show();
    pTable->setLeftMargin( 5 );
    pTable->setSorting( false );

    uint n = 0;
    for( ADN_Maintenance_Data::IT_WorkingSchemeInfo_Vector it = data_.vWorkingSchemes_.begin(); it != data_.vWorkingSchemes_.end(); ++it )
    {
        pTable->horizontalHeader()->setLabel( n, tr( "Shift " ) + QString::number( ( **it ).nIdx_.GetData() ) );
        pTable->setColumnStretchable( n, true );
        ++n;
    }

    pTable->verticalHeader()->setLabel( 0, tr( "Work duration" ) );
    pTable->verticalHeader()->setLabel( 1, tr( "Warning after" ) );

    n = 0;
    for( ADN_Maintenance_Data::IT_WorkingSchemeInfo_Vector it = data_.vWorkingSchemes_.begin(); it != data_.vWorkingSchemes_.end(); ++it )
    {
        ADN_Maintenance_Data::WorkingSchemeInfo& modifiers = **it;
        builder.AddTableCell< ADN_TableItem_Int >( pTable, &modifiers, 0, n, modifiers.nWorkTime_, eGreaterZero );
        builder.AddTableCell< ADN_TableItem_TimeField >( pTable, &modifiers, 1, n, modifiers.warningDelay_ );
        ++n;
    }
}
