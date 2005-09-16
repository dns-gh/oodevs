// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-18 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Log_GUI.cpp $
// $Author: Ape $
// $Modtime: 21/04/05 16:51 $
// $Revision: 6 $
// $Workfile: ADN_Log_GUI.cpp $
//
// *****************************************************************************

#include "ADN_pch.h"
#include "ADN_Log_GUI.h"

#include "ADN_Log_Data.h"
#include "ADN_GuiBuilder.h"
#include "ADN_EditLine.h"
#include "ADN_Table.h"
#include "ADN_TableItem_Edit.h"

#include "ENT/ENT_Tr.h"

#include <qgroupbox.h>
#include <qhgroupbox.h>
#include <qhbox.h>
#include <qgrid.h>
#include <qlabel.h>
#include <qlayout.h>


// -----------------------------------------------------------------------------
// Name: ADN_Log_GUI constructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Log_GUI::ADN_Log_GUI( ADN_Log_Data& data )
: ADN_GUI_ABC( "ADN_Log_GUI" )
, data_      ( data )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Log_GUI destructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Log_GUI::~ADN_Log_GUI()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Log_GUI::Build
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
void ADN_Log_GUI::Build()
{
    assert( pMainWidget_ == 0 );
    ADN_GuiBuilder builder;

    // Create the main widget.
    pMainWidget_ = new QWidget( 0 );

    QGroupBox* pGroup = new QHGroupBox( tr( "Temps de bord�e" ), pMainWidget_ );

    ADN_Table* pTable = builder.CreateTable( pGroup );
    pTable->setNumCols( data_.vWorkTimeModifiers_.size() );
    pTable->setNumRows( 3 );
    pTable->verticalHeader()->show();
    pTable->setLeftMargin( 5 );
    pTable->setSorting( false );

    uint n = 0;
    for( ADN_Log_Data::IT_WorkTimeModifiersInfo_Vector it = data_.vWorkTimeModifiers_.begin(); it != data_.vWorkTimeModifiers_.end(); ++it )
    {
        pTable->horizontalHeader()->setLabel( n, ENT_Tr::ConvertFromTempsBordee( ( **it ).nType_, ENT_Tr::eToTr ).c_str() );
        pTable->setColumnStretchable( n, true );
        ++n;
    }

    pTable->verticalHeader()->setLabel( 0, tr( "Speed factor" ) );
    pTable->verticalHeader()->setLabel( 1, tr( "Sorting time factor" ) );
    pTable->verticalHeader()->setLabel( 2, tr( "Treating time factor" ) );

    n = 0;
    for( ADN_Log_Data::IT_WorkTimeModifiersInfo_Vector it = data_.vWorkTimeModifiers_.begin(); it != data_.vWorkTimeModifiers_.end(); ++it )
    {
        ADN_Log_Data::WorkTimeModifiersInfo& modifiers = **it;
        builder.AddTableCell< ADN_TableItem_Double >( pTable, &modifiers, 0, n, modifiers.rRepairModifier_, eGreaterZero );
        builder.AddTableCell< ADN_TableItem_Double >( pTable, &modifiers, 1, n, modifiers.rSortModifier_, eGreaterZero );
        builder.AddTableCell< ADN_TableItem_Double >( pTable, &modifiers, 2, n, modifiers.rTreatModifier_, eGreaterZero );
        ++n;
    }

    // Layout
    QVBoxLayout* pLayout = new QVBoxLayout( pMainWidget_, 10, 10 );
    pLayout->addWidget( pGroup );
}
