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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Maintenance_GUI.cpp $
// $Author: Ape $
// $Modtime: 21/04/05 16:51 $
// $Revision: 6 $
// $Workfile: ADN_Maintenance_GUI.cpp $
//
// *****************************************************************************

#include "ADN_pch.h"
#include "ADN_Maintenance_GUI.h"

#include "ADN_Maintenance_Data.h"
#include "ADN_GuiBuilder.h"
#include "ADN_EditLine.h"
#include "ADN_Table.h"
#include "ADN_TableItem_Edit.h"
#include "ADN_TableItem_TimeField.h"

#include "ENT/ENT_Tr.h"

#include <qgroupbox.h>
#include <qhgroupbox.h>
#include <qhbox.h>
#include <qgrid.h>
#include <qlabel.h>
#include <qlayout.h>


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
    assert( pMainWidget_ == 0 );
    ADN_GuiBuilder builder;

    // Create the main widget.
    pMainWidget_ = new QWidget( 0 );

    QGroupBox* pGroup = new QHGroupBox( tr( "Regimes de travail" ), pMainWidget_ );

    ADN_Table* pTable = builder.CreateTable( pGroup );
    pTable->setNumCols( data_.vWorkingSchemes_.size() );
    pTable->setNumRows( 2 );
    pTable->verticalHeader()->show();
    pTable->setLeftMargin( 5 );
    pTable->setSorting( false );

    uint n = 0;
    for( ADN_Maintenance_Data::IT_WorkingSchemeInfo_Vector it = data_.vWorkingSchemes_.begin(); it != data_.vWorkingSchemes_.end(); ++it )
    {
        pTable->horizontalHeader()->setLabel( n, tr( "Regime" ) + QString::number( ( **it ).nIdx_.GetData() ) );
        pTable->setColumnStretchable( n, true );
        ++n;
    }

    pTable->verticalHeader()->setLabel( 0, tr( "Work time" ) );
    pTable->verticalHeader()->setLabel( 1, tr( "Warning Delay" ) );

    n = 0;
    for( ADN_Maintenance_Data::IT_WorkingSchemeInfo_Vector it = data_.vWorkingSchemes_.begin(); it != data_.vWorkingSchemes_.end(); ++it )
    {
        ADN_Maintenance_Data::WorkingSchemeInfo& modifiers = **it;
        builder.AddTableCell< ADN_TableItem_Int >( pTable, &modifiers, 0, n, modifiers.nWorkTime_, eGreaterZero );
        builder.AddTableCell< ADN_TableItem_TimeField >( pTable, &modifiers, 1, n, modifiers.warningDelay_ );
        ++n;
    }

    // Layout
    QVBoxLayout* pLayout = new QVBoxLayout( pMainWidget_, 10, 10 );
    pLayout->addWidget( pGroup );
}
