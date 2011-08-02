// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_AvailabilityWarningTable.h"
#include "ADN_AvailabilityWarning.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_TableItem_Edit.h"
#include "ADN_Validator.h"

#include <Qt3Support/q3popupmenu.h>

class ADN_Connector_WarningTable
    : public ADN_Connector_Table_ABC
{
public:
    ADN_Connector_WarningTable( ADN_AvailabilityWarningTable& tab )
        : ADN_Connector_Table_ABC( tab, true, "ADN_Connector_WarningTable" )
    {}

    void  AddSubItems( int i, void* pObj )
    {
        assert( pObj );
        ADN_TableItem_Int *pItem = new ADN_TableItem_Int( &tab_, pObj );
        pItem->UseColor( true );
        pItem->SetRangeForColor( 0, 100 );

        // add a new row & set new values
        tab_.setItem( i, 0, pItem );
        pItem->GetValidator().setRange( 0, 100 );
        pItem->GetConnector().Connect( &static_cast<ADN_AvailabilityWarning*>( pObj )->percentage_ );
    }

    bool LessComparison( void* pL, void* pR ) const
    {
        ADN_AvailabilityWarning* pLInfo = static_cast<ADN_AvailabilityWarning*>( pL );
        ADN_AvailabilityWarning* pRInfo = static_cast<ADN_AvailabilityWarning*>( pR );
        return pLInfo->percentage_.GetData() < pRInfo->percentage_.GetData();
    }
};

// -----------------------------------------------------------------------------
// Name: ADN_AvailabilityWarningTable constructor
// Created: SBO 2006-08-03
// -----------------------------------------------------------------------------
ADN_AvailabilityWarningTable::ADN_AvailabilityWarningTable( QWidget* pParent )
    : ADN_Table2( pParent, "ADN_AvailabilityWarningTable" )
{
    // Selection and sorting.
    setSorting( true );
    setSelectionMode( Q3Table::NoSelection );
    setShowGrid( false );
    setLeftMargin( 0 );
    verticalHeader()->hide();

    setNumCols( 1 );
    setNumRows( 0 );
    setColumnStretchable( 0, true );
    horizontalHeader()->setLabel( 0, tr( "Percentage" ) );

    // Create the connector.
    pConnector_ = new ADN_Connector_WarningTable( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_AvailabilityWarningTable destructor
// Created: SBO 2006-08-03
// -----------------------------------------------------------------------------
ADN_AvailabilityWarningTable::~ADN_AvailabilityWarningTable()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_AvailabilityWarningTable::OnContextMenu
// Created: SBO 2006-08-03
// -----------------------------------------------------------------------------
void ADN_AvailabilityWarningTable::OnContextMenu( int, int, const QPoint& pt )
{
    Q3PopupMenu menu( this );
    menu.insertItem( tr( "New warning"), 0 );
    menu.insertItem( tr( "Delete warning"), 1 );
    menu.setItemEnabled( 1, GetCurrentData() != 0 );

    const int nMenu = menu.exec( pt );
    if( nMenu == 0 )
        CreateNewElement();
    else if( nMenu == 1 )
        DeleteCurrentElement();
}

// -----------------------------------------------------------------------------
// Name: ADN_AvailabilityWarningTable::CreateNewElement
// Created: SBO 2006-08-03
// -----------------------------------------------------------------------------
void ADN_AvailabilityWarningTable::CreateNewElement()
{
    ADN_AvailabilityWarning* pNew = new ADN_AvailabilityWarning();
    ADN_Connector_Vector_ABC* pCTable = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
    pCTable->AddItem( pNew );
    pCTable->AddItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_AvailabilityWarningTable::DeleteCurrentElement
// Created: SBO 2006-08-03
// -----------------------------------------------------------------------------
void ADN_AvailabilityWarningTable::DeleteCurrentElement()
{
    ADN_AvailabilityWarning* pCurrent = (ADN_AvailabilityWarning*)GetCurrentData();
    if( pCurrent != 0 )
        static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( pCurrent );
}
