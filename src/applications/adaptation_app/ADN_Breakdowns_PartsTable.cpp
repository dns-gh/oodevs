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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Breakdowns_PartsTable.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 16:56 $
// $Revision: 6 $
// $Workfile: ADN_Breakdowns_PartsTable.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Breakdowns_PartsTable.h"
#include "ADN_Equipement_Data.h"
#include "ADN_Breakdowns_Data.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_TableItem_Edit.h"
#include "ADN_Validator.h"

typedef ADN_Breakdowns_Data::RepairPartInfo RepairPartInfo;


//-----------------------------------------------------------------------------
// Internal table connector
//-----------------------------------------------------------------------------
class ADN_Connector_PartsTable
    : public ADN_Connector_Table_ABC
{
public:
    ADN_Connector_PartsTable( ADN_Breakdowns_PartsTable& tab )
        : ADN_Connector_Table_ABC( tab, false, "ADN_Automata_PartsTable" )
    {}

    void AddSubItems( int i, void* pObj )
    {
        assert( pObj );

        // Add a new row.
        ADN_TableItem_String* pItemName = new ADN_TableItem_String( &tab_, pObj, Q3TableItem::Never );
        ADN_TableItem_Int* pItemNbr = new ADN_TableItem_Int( &tab_, pObj );
        tab_.setItem( i, 0, pItemName );
        tab_.setItem( i, 1, pItemNbr );
        pItemNbr->GetValidator().setBottom( 1 );

        // Connect the items.
        pItemName->GetConnector().Connect( &static_cast<RepairPartInfo*>(pObj)->ptrPart_.GetData()->strName_ );
        pItemNbr->GetConnector().Connect( &static_cast<RepairPartInfo*>(pObj)->nNbr_ );
    }
};


// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_PartsTable constructor
// Created: APE 2005-01-07
// -----------------------------------------------------------------------------
ADN_Breakdowns_PartsTable::ADN_Breakdowns_PartsTable( QWidget* pParent )
: ADN_Table2( pParent, "ADN_Breakdowns_PartsTable" )
{
    // Selection and sorting.
    setSorting( true );
    setSelectionMode( Q3Table::NoSelection );
    setShowGrid( false );
    setLeftMargin( 0 );

    // Hide the vertical header.
    verticalHeader()->hide();

    // Setup 2 columns.
    setNumCols( 2 );
    setNumRows( 0 );
    setColumnStretchable( 0, true );
    setColumnStretchable( 1, true );

    horizontalHeader()->setLabel( 0, tr( "Name" ) );
    horizontalHeader()->setLabel( 1, tr( "Nbr required" ) );

    // Create the connector.
    pConnector_ = new ADN_Connector_PartsTable( *this );
}


// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_PartsTable destructor
// Created: APE 2005-01-07
// -----------------------------------------------------------------------------
ADN_Breakdowns_PartsTable::~ADN_Breakdowns_PartsTable()
{
    delete pConnector_;
}


// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_PartsTable::OnContextMenu
// Created: APE 2005-01-10
// -----------------------------------------------------------------------------
void ADN_Breakdowns_PartsTable::OnContextMenu( int /*nRow*/, int /*nCol*/, const QPoint& pt )
{
    Q3PopupMenu menu( this );
    Q3PopupMenu subMenu( &menu );

    ADN_Equipement_Data::T_CategoryInfos_Vector& parts = ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetDotation( eDotationFamily_Piece ).categories_;
    for( ADN_Equipement_Data::IT_CategoryInfos_Vector it = parts.begin(); it != parts.end(); ++it )
    {
        if( this->Contains( **it ) )
            continue;
        subMenu.insertItem( (*it)->strName_.GetData().c_str(), (int)(*it) );
    }
    ADN_Tools::SortMenu( subMenu );

    menu.insertItem( tr( "Add required part"), &subMenu );
    menu.insertItem( tr( "Remove required part"), 0 );
    menu.setItemEnabled( 0, GetCurrentData() != 0 );

    int nMenuResult = menu.exec( pt );

    if( nMenuResult == -1 )
        return;
    else if( nMenuResult == 0 )
    {
        // Delete the current element.
        RepairPartInfo* pCurrentPart = (RepairPartInfo*)GetCurrentData();
        if( pCurrentPart != 0 )
            static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( pCurrentPart );
    }
    else
    {
        // Create a new element
        RepairPartInfo* pNewInfo = new RepairPartInfo();
        pNewInfo->ptrPart_ = (ADN_Equipement_Data::CategoryInfo*)nMenuResult;
        ADN_Connector_Vector_ABC* pCTable = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
        pCTable->AddItem( pNewInfo );
        pCTable->AddItem( 0 );
    }
}


// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_PartsTable::Contains
// Created: AGN 2004-02-25
// -----------------------------------------------------------------------------
bool ADN_Breakdowns_PartsTable::Contains( ADN_Equipement_Data::CategoryInfo& category )
{
    int n = 0;
    while( item( n, 1 ) != 0 )
    {
        ADN_TableItem_ABC* pItem = static_cast<ADN_TableItem_ABC*>( item( n, 1 ) );
        RepairPartInfo* pInfos = static_cast<RepairPartInfo*>( pItem->GetData() );
        if( pInfos->ptrPart_.GetData() == &category )
            return true;
        ++n;
    }
    return false;
}
