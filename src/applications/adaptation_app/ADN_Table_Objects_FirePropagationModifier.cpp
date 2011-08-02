// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Table_Objects_FirePropagationModifier.h"
#include "ADN_Objects_Data.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_TableItem_Edit.h"

typedef ADN_Objects_Data::ADN_CapacityInfos_FirePropagationModifier::ModifierByFireClass ModifierByFireClass;

//-----------------------------------------------------------------------------
// Internal Table connector to be connected with
//-----------------------------------------------------------------------------
class ADN_Table_Objects_FirePropagationModifier_Connector
    : public ADN_Connector_Table_ABC
{
public:
    ADN_Table_Objects_FirePropagationModifier_Connector( ADN_Table_Objects_FirePropagationModifier& tab )
        : ADN_Connector_Table_ABC( tab, false )
    {}

    void AddSubItems( int i, void *obj )
    {
        assert(obj);
        ModifierByFireClass* pModifier = static_cast< ModifierByFireClass* >( obj );
        if( pModifier->ptrFireClass_.GetData()->isSurface_.GetData() )
        {
            ADN_TableItem_String *pNameItem = new ADN_TableItem_String( &tab_, obj );
            ADN_TableItem_Int *pIgnitionThresholdItem = new ADN_TableItem_Int( &tab_, obj );
            ADN_TableItem_Int *pMaxCombustionEnergyItem = new ADN_TableItem_Int( &tab_, obj );
            // add a new row & set new values
            tab_.setItem( i, 0, pNameItem );
            tab_.setItem( i, 1, pIgnitionThresholdItem );
            tab_.setItem( i, 2, pMaxCombustionEnergyItem );
            tab_.setColumnWidth( 1, 140 );
            tab_.setColumnWidth( 2, 140 );
            // disable first column
            pNameItem->setEnabled( false );
            // connect items & datas
            pNameItem->GetConnector().Connect( &pModifier->ptrFireClass_.GetData()->strName_ );
            pIgnitionThresholdItem->GetConnector().Connect( &pModifier->ignitionThreshold_ );
            pMaxCombustionEnergyItem->GetConnector().Connect( &pModifier->maxCombustionEnergy_ );
        }
        else
            tab_.hideRow( i );
    }
};

//-----------------------------------------------------------------------------
// Name: ADN_Table_Objects_FirePropagationModifier constructor
// Created: BCI 2010-12-02
//-----------------------------------------------------------------------------
ADN_Table_Objects_FirePropagationModifier::ADN_Table_Objects_FirePropagationModifier( QWidget* pParent )
    : ADN_Table2( pParent, "ADN_Table_Objects_FirePropagationModifier" )
{
    // peut etre selectionne & trie
    setSorting( true );
    setSelectionMode( Q3Table::NoSelection );
    setShowGrid( false );
    setMinimumHeight( 80 );
    setLeftMargin( 0 );
    // hide vertical header
    verticalHeader()->hide();
    // tab with 2 columns
    setNumCols( 3 );
    setNumRows( 0 );
    setColumnStretchable( 0, true );
    setColumnStretchable( 1, true );
    horizontalHeader()->setLabel( 0, tr( "Fire model" ) );
    horizontalHeader()->setLabel( 1, tr( "Ignition threshold" ) );
    horizontalHeader()->setLabel( 2, tr( "Max combustion energy" ) );
    // connector creation
    pConnector_ = new ADN_Table_Objects_FirePropagationModifier_Connector( *this );
}

//-----------------------------------------------------------------------------
// Name: ADN_Table_Objects_FirePropagationModifier destructor
// Created: BCI 2010-12-02
//-----------------------------------------------------------------------------
ADN_Table_Objects_FirePropagationModifier::~ADN_Table_Objects_FirePropagationModifier()
{
    delete pConnector_;
}
