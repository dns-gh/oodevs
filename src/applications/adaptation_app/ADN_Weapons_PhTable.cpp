// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-01-07 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Weapons_PhTable.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 16:50 $
// $Revision: 6 $
// $Workfile: ADN_Weapons_PhTable.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Weapons_PhTable.h"
#include "ADN_Weapons_Data.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_TableItem_Edit.h"
#include "ADN_Validator.h"

typedef ADN_Weapons_Data::PhInfos PhInfos;


//-----------------------------------------------------------------------------
// Internal Table connector to be connected with T_ModificatorComposanteInfos_Vector
//-----------------------------------------------------------------------------
class ADN_Connector_PhTable
    : public ADN_Connector_Table_ABC
{
public:
    ADN_Connector_PhTable( ADN_Weapons_PhTable& tab )
        : ADN_Connector_Table_ABC( tab, true, "ADN_Connector_PhTable" )
    {}

    void  AddSubItems( int i, void* pObj )
    {
        assert( pObj );
        ADN_TableItem_Int *pItemDistance = new ADN_TableItem_Int( &tab_, pObj );
        ADN_TableItem_Double *pItemPerc = new ADN_TableItem_Double( &tab_, pObj );
        pItemPerc->UseColor( true );
        pItemPerc->SetRangeForColor( 0.0, 100.0 );

        // add a new row & set new values
        tab_.setItem( i, 0, pItemDistance );
        tab_.setItem( i, 1, pItemPerc );

        // set table item properties
        pItemDistance->GetValidator().setBottom( 0 );
        pItemPerc->GetValidator().setRange( 0, 100, 5 );

        // connect items & datas
        pItemDistance->GetConnector().Connect( &static_cast<PhInfos*>(pObj)->nDistance_ );
        pItemPerc->GetConnector().Connect( &static_cast<PhInfos*>(pObj)->rPerc_ );
    }

    bool LessComparison( void* pL, void* pR ) const
    {
        PhInfos* pLInfo = static_cast<PhInfos*>( pL );
        PhInfos* pRInfo = static_cast<PhInfos*>( pR );
        return pLInfo->nDistance_.GetData() < pRInfo->nDistance_.GetData();
    }
};


// -----------------------------------------------------------------------------
// Name: ADN_Weapons_PhTable constructor
// Created: APE 2005-01-07
// -----------------------------------------------------------------------------
ADN_Weapons_PhTable::ADN_Weapons_PhTable( QWidget* pParent )
: ADN_Table2( pParent, "ADN_Weapons_PhTable" )
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

    horizontalHeader()->setLabel(0, tr( "Distance" ) );
    horizontalHeader()->setLabel(1, tr( "Percentage (PH)" ) );

    // Create the connector.
    pConnector_ = new ADN_Connector_PhTable( *this );
}


// -----------------------------------------------------------------------------
// Name: ADN_Weapons_PhTable destructor
// Created: APE 2005-01-07
// -----------------------------------------------------------------------------
ADN_Weapons_PhTable::~ADN_Weapons_PhTable()
{
    delete pConnector_;
}


// -----------------------------------------------------------------------------
// Name: ADN_Weapons_PhTable::OnContextMenu
// Created: APE 2005-01-10
// -----------------------------------------------------------------------------
void ADN_Weapons_PhTable::OnContextMenu( int /*nRow*/, int /*nCol*/, const QPoint& pt )
{
    Q3PopupMenu menu( this );
    menu.insertItem( tr( "New Ph"), 0 );
    menu.insertItem( tr( "Delete Ph"), 1 );
    menu.setItemEnabled( 1, GetCurrentData() != 0 );

    int nMenu = menu.exec( pt );

    switch ( nMenu )
    {
    case 0:
        {
            // create new munition & add it to the list
            CreateNewElement();
            break;
        }
    case 1:
        {
            DeleteCurrentElement();
            break;
        }
    default:
        break;
    }
}


// -----------------------------------------------------------------------------
// Name: ADN_Weapons_PhTable::CreateNewElement
// Created: APE 2005-01-10
// -----------------------------------------------------------------------------
void ADN_Weapons_PhTable::CreateNewElement()
{
    PhInfos* pNewInfo = new PhInfos();
    ADN_Connector_Vector_ABC* pCTable = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
    pCTable->AddItem( pNewInfo );
    pCTable->AddItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_PhTable::doValueChanged
// Created: JSR 2010-04-28
// -----------------------------------------------------------------------------
void ADN_Weapons_PhTable::doValueChanged( int row, int col )
{
    ADN_Table2::doValueChanged( row, col );
    PhInfos* pCurPh = (PhInfos*)GetCurrentData();
    if( pCurPh != 0 )
    {
        pCurPh->ApplyPhModifiers();
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_PhTable::DeleteCurrentElement
// Created: APE 2005-01-10
// -----------------------------------------------------------------------------
void ADN_Weapons_PhTable::DeleteCurrentElement()
{
    PhInfos* pCurPh = (PhInfos*)GetCurrentData();
    if( pCurPh != 0 )
    {
        // remove current data from list
        // take care cause pCurData_ can change!!
        static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( pCurPh );
    }
}
