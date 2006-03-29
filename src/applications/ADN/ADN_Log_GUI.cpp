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
#include "ADN_TableItem_TimeField.h"
#include "ADN_TableItem_CheckItem.h"

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

    QGroupBox* pGroup = new QHGroupBox( tr( "Temps de bordée" ), pMainWidget_ );

    ADN_Table* pTable = builder.CreateTable( pGroup );
    pTable->setNumCols( 2 );
    pTable->setNumRows( data_.vWorkTimeModifiers_.size() );
    pTable->verticalHeader()->show();
    pTable->setLeftMargin( 5 );
    pTable->setSorting( false );

    uint n = 0;
    for( ADN_Log_Data::IT_WorkTimeModifiersInfo_Vector it = data_.vWorkTimeModifiers_.begin(); it != data_.vWorkTimeModifiers_.end(); ++it )
    {
        pTable->verticalHeader()->setLabel( n, ENT_Tr::ConvertFromTempsBordee( ( **it ).nType_, ENT_Tr::eToTr ).c_str() );
        pTable->setColumnStretchable( n, true );
        ++n;
    }

    pTable->horizontalHeader()->setLabel( 0, tr( "Specify delay" ) );
    pTable->horizontalHeader()->setLabel( 1, tr( "Warning delay" ) );

    n = 0;
    for( ADN_Log_Data::IT_WorkTimeModifiersInfo_Vector it = data_.vWorkTimeModifiers_.begin(); it != data_.vWorkTimeModifiers_.end(); ++it )
    {
        ADN_Log_Data::WorkTimeModifiersInfo& modifiers = **it;
        
        ADN_TableItem_CheckItem* pWorkingTimeSet = new ADN_TableItem_CheckItem( pTable, *it );
        pTable->setItem( n, 0, pWorkingTimeSet );
        pWorkingTimeSet->GetConnector().Connect( &modifiers.bWorkingTimeSet_ );
        builder.AddTableCell< ADN_TableItem_TimeField >( pTable, &modifiers, n, 1, modifiers.workingTime_ );
        ++n;
    }

    // Layout
    QVBoxLayout* pLayout = new QVBoxLayout( pMainWidget_, 10, 10 );
    pLayout->addWidget( pGroup );
}
