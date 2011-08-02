// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_UrbanModifiersTable.h"
#include "ADN_Equipement_Data.h"
#include "ADN_Equipement_GUI.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_TableItem_Edit.h"

using namespace helpers;

//-----------------------------------------------------------------------------
// Internal Table connector to be connected with
//-----------------------------------------------------------------------------
class ADN_UrbanModifiersTable_Connector : public ADN_Connector_Table_ABC
{

public:
    explicit ADN_UrbanModifiersTable_Connector( ADN_UrbanModifiersTable& tab )
        : ADN_Connector_Table_ABC( tab, false )
    {}

    void AddSubItems( int i, void* obj )
    {
        assert( obj );
        ADN_TableItem_String *pItemString = 0;
        ADN_TableItem_Double *pItemDouble = 0;
        // add a new row & set new values
        tab_.setItem( i, 0, pItemString = new ADN_TableItem_String( &tab_, obj ) );
        tab_.setItem( i, 1, pItemDouble = new ADN_TableItem_Double( &tab_, obj ) );
        // disable first column
        pItemString->setEnabled( false );
        // set table item properties
        pItemDouble->GetValidator().setRange( 0, 1, 2 );
        // connect items & datas
        pItemString->GetConnector().Connect( &static_cast< helpers::ADN_UrbanAttritionInfos* >( obj )->ptrMaterial_.GetData()->strName_ );
        pItemDouble->GetConnector().Connect( &static_cast< helpers::ADN_UrbanAttritionInfos* >( obj )->rCoeff_ );
    }
};

//-----------------------------------------------------------------------------
// Name: ADN_UrbanModifiersTable constructor
// Created: SLG 2010-04-13
//-----------------------------------------------------------------------------
ADN_UrbanModifiersTable::ADN_UrbanModifiersTable( QWidget* pParent, ADN_Connector_ABC*& pGuiConnector )
    : ADN_Table2( pParent, "ADN_UrbanModifiersTable" )
{
    // peut etre selectionne & trie
    setSorting( true );
    setSelectionMode( Q3Table::NoSelection );
    setShowGrid( false );
    setLeftMargin( 0 );
    // hide vertical header
    verticalHeader()->hide();
    // tab with 2 columns
    setNumCols( 2 );
    setNumRows( 0 );
    setColumnStretchable( 0, true );
    setColumnStretchable( 1, true );
    horizontalHeader()->setLabel( 0, tr( "Material" ) );
    horizontalHeader()->setLabel( 1, tr( "Modifiers" ) );
    // connector creation
    pConnector_ = new ADN_UrbanModifiersTable_Connector( *this );
    pGuiConnector = pConnector_;
}

//-----------------------------------------------------------------------------
// Name: ADN_UrbanModifiersTable destructor
// Created: SLG 2010-04-13
//-----------------------------------------------------------------------------
ADN_UrbanModifiersTable::~ADN_UrbanModifiersTable()
{
    delete pConnector_;
}
