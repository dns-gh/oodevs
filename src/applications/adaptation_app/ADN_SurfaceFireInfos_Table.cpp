// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_SurfaceFireInfos_Table.h"
#include "ADN_FireClass_Data.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_TableItem_Edit.h"

//-----------------------------------------------------------------------------
// Internal Table connector to be connected with
//-----------------------------------------------------------------------------
class ADN_SurfaceFireInfos_Table_Connector
    : public ADN_Connector_Table_ABC
{
public:
    ADN_SurfaceFireInfos_Table_Connector( ADN_SurfaceFireInfos_Table& tab )
        : ADN_Connector_Table_ABC( tab, false )
    {}

    void AddSubItems( int i, void *obj )
    {
        assert( obj );
        ADN_TableItem_String *pItemString = new ADN_TableItem_String( &tab_, obj );
        ADN_TableItem_Int *pItemIgnition = new ADN_TableItem_Int( &tab_, obj );
        ADN_TableItem_Int *pItemCombustion = new ADN_TableItem_Int( &tab_, obj );
        // add a new row & set new values
        tab_.setItem( i, 0, pItemString );
        tab_.setItem( i, 1, pItemIgnition );
        tab_.setItem( i, 2, pItemCombustion );
        // disable first column
        pItemString->setEnabled( false );
        // connect items & datas
        pItemString->GetConnector().Connect( &static_cast< ADN_FireClass_Data::FireSurfaceInfos* >( obj )->strName_ );
        pItemIgnition->GetConnector().Connect( &static_cast< ADN_FireClass_Data::FireSurfaceInfos* >( obj )->ignitionThreshold_ );
        pItemCombustion->GetConnector().Connect( &static_cast< ADN_FireClass_Data::FireSurfaceInfos* >( obj )->maxCombustionEnergy_ );
    }
};

//-----------------------------------------------------------------------------
// Name: ADN_SurfaceFireInfos_Table constructor
// Created: BCI 2010-12-03
//-----------------------------------------------------------------------------
ADN_SurfaceFireInfos_Table::ADN_SurfaceFireInfos_Table( QWidget* pParent )
    : ADN_Table2( pParent, "ADN_SurfaceFireInfos_Table" )
{
    // peut etre selectionne & trie
    setSorting( true );
    setSelectionMode( Q3Table::NoSelection );
    setShowGrid( false );
    setLeftMargin( 0 );
    // hide vertical header
    verticalHeader()->hide();
    // tab with 3 columns
    setNumCols( 3 );
    setNumRows( 0 );
    setColumnStretchable( 0, true );
    setColumnStretchable( 1, true );
    setColumnStretchable( 2, true );
    horizontalHeader()->setLabel( 0, tr( "Ground type" ) );
    horizontalHeader()->setLabel( 1, tr( "Ignition threshold" ) );
    horizontalHeader()->setLabel( 2, tr( "Max combustion energy" ) );
    // connector creation
    pConnector_ = new ADN_SurfaceFireInfos_Table_Connector( *this );
}

//-----------------------------------------------------------------------------
// Name: ADN_SurfaceFireInfos_Table destructor
// Created: JSR 2010-12-03
//-----------------------------------------------------------------------------
ADN_SurfaceFireInfos_Table::~ADN_SurfaceFireInfos_Table()
{
    delete pConnector_;
}
