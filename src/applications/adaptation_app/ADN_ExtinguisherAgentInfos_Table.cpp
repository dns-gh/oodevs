// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ExtinguisherAgentInfos_Table.h"
#include "ADN_ExtinguisherAgentInfos.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_TableItem_Edit.h"

//-----------------------------------------------------------------------------
// Internal Table connector to be connected with
//-----------------------------------------------------------------------------
class ADN_ExtinguisherAgentInfos_Table_Connector
    : public ADN_Connector_Table_ABC
{
public:
    ADN_ExtinguisherAgentInfos_Table_Connector( ADN_ExtinguisherAgentInfos_Table& tab )
        : ADN_Connector_Table_ABC( tab, false )
    {}

    void AddSubItems( int i, void *obj )
    {
        assert( obj );
        ADN_TableItem_String *pItemString = new ADN_TableItem_String( &tab_, obj );
        ADN_TableItem_Int *pItemInt = new ADN_TableItem_Int( &tab_, obj );
        // add a new row & set new values
        tab_.setItem( i, 0, pItemString );
        tab_.setItem( i, 1, pItemInt );
        // disable first column
        pItemString->setEnabled( false );
        // connect items & datas
        pItemString->GetConnector().Connect( &static_cast< ADN_ExtinguisherAgentInfos* >( obj )->ptrAgent_.GetData()->strName_ );
        pItemInt->GetConnector().Connect( &static_cast< ADN_ExtinguisherAgentInfos* >( obj )->heatDecreaseRate_ );
    }
};

//-----------------------------------------------------------------------------
// Name: ADN_ExtinguisherAgentInfos_Table constructor
// Created: BCI 2010-12-03
//-----------------------------------------------------------------------------
ADN_ExtinguisherAgentInfos_Table::ADN_ExtinguisherAgentInfos_Table( QWidget* pParent )
    : ADN_Table2( pParent, "ADN_ExtinguisherAgentInfos_Table" )
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
    horizontalHeader()->setLabel( 0, tr( "Agent" ) );
    horizontalHeader()->setLabel( 1, tr( "Heat decrease rate" ) );
    // connector creation
    pConnector_ = new ADN_ExtinguisherAgentInfos_Table_Connector( *this );
}

//-----------------------------------------------------------------------------
// Name: ADN_ExtinguisherAgentInfos_Table destructor
// Created: BCI 2010-12-03
//-----------------------------------------------------------------------------
ADN_ExtinguisherAgentInfos_Table::~ADN_ExtinguisherAgentInfos_Table()
{
    delete pConnector_;
}
