// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-22 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Supply_TrucksDoubleTable.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 16:56 $
// $Revision: 4 $
// $Workfile: ADN_Supply_TrucksDoubleTable.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Supply_TrucksDoubleTable.h"

#include <Qt3Support/q3popupmenu.h>

#include "ADN_Supply_Data.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_TableItem_Edit.h"
#include "ADN_TableItem_TimeField.h"
#include "ADN_Validator.h"

typedef ADN_Supply_Data::ConvoyInfo< ADN_Type_Double > ConvoyInfo;


//-----------------------------------------------------------------------------
// Internal table connector
//-----------------------------------------------------------------------------
class ADN_Connector_TrucksDoubleTable
    : public ADN_Connector_Table_ABC
{
public:
    ADN_Connector_TrucksDoubleTable( ADN_Supply_TrucksDoubleTable& tab )
        : ADN_Connector_Table_ABC( tab, false, "ADN_Connector_TrucksDoubleTable" )
    {}

    void AddSubItems( int i, void* pObj )
    {
        assert( pObj );

        // Add a new row.
        ADN_TableItem_Int* pItemNbrTrucks = new ADN_TableItem_Int( &tab_, pObj );
        ADN_TableItem_Double* pItemDouble = new ADN_TableItem_Double( &tab_, pObj );
        tab_.setItem( i, 0, pItemNbrTrucks );
        tab_.setItem( i, 1, pItemDouble );
        pItemNbrTrucks->GetValidator().setBottom( 1 );

        // Connect the items.
        pItemNbrTrucks->GetConnector().Connect( &static_cast<ConvoyInfo*>(pObj)->nNbrTrucks_ );
        pItemDouble->GetConnector().Connect( &static_cast<ConvoyInfo*>(pObj)->value_ );
    }
};


// -----------------------------------------------------------------------------
// Name: ADN_Supply_TrucksDoubleTable constructor
// Created: APE 2005-01-07
// -----------------------------------------------------------------------------
ADN_Supply_TrucksDoubleTable::ADN_Supply_TrucksDoubleTable( QWidget* pParent )
: ADN_Table2( pParent, "ADN_Supply_TrucksDoubleTable" )
{
    // Selection and sorting.
    setSorting( true );
    setSelectionMode( Q3Table::NoSelection );
    setShowGrid( false );
    setLeftMargin( 0 );

    this->setMaximumHeight( 300 );

    // Hide the vertical header.
    verticalHeader()->hide();

    // Setup 2 columns.
    setNumCols( 2 );
    setNumRows( 0 );
    setColumnStretchable( 0, true );
    setColumnStretchable( 1, true );

    horizontalHeader()->setLabel( 0, tr( "Trucks qty" ) );
    horizontalHeader()->setLabel( 1, tr( "Max speed modifier" ) );

    // Create the connector.
    pConnector_ = new ADN_Connector_TrucksDoubleTable( *this );
}


// -----------------------------------------------------------------------------
// Name: ADN_Supply_TrucksDoubleTable destructor
// Created: APE 2005-01-07
// -----------------------------------------------------------------------------
ADN_Supply_TrucksDoubleTable::~ADN_Supply_TrucksDoubleTable()
{
    delete pConnector_;
}


// -----------------------------------------------------------------------------
// Name: ADN_Supply_TrucksDoubleTable::OnContextMenu
// Created: APE 2005-01-10
// -----------------------------------------------------------------------------
void ADN_Supply_TrucksDoubleTable::OnContextMenu( int /*nRow*/, int /*nCol*/, const QPoint& pt )
{
    Q3PopupMenu menu( this );

    menu.insertItem( tr( "New" ), 0 );
    menu.insertItem( tr( "Delete" ), 1 );
    menu.setItemEnabled( 1, GetCurrentData() != 0 );

    int nMenuResult = menu.exec( pt );

    if( nMenuResult == -1 )
        return;
    else if( nMenuResult == 1 )
    {
        // Delete the current element.
        ConvoyInfo* pCurrentPart = (ConvoyInfo*)GetCurrentData();
        if( pCurrentPart != 0 )
            static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( pCurrentPart );
    }
    else
    {
        assert( nMenuResult == 0 );
        // Create a new element
        ConvoyInfo* pNewInfo = new ConvoyInfo( 1. );
        ADN_Connector_Vector_ABC* pCTable = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
        pCTable->AddItem( pNewInfo );
        pCTable->AddItem( 0 );
    }
}


