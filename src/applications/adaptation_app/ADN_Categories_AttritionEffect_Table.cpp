// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Categories_AttritionEffect_Table.h"
#include "ADN_Categories_Data.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_Categories_GUI.h"
#include "ADN_Equipement_GUI.h"
#include "ADN_TableItem_Edit.h"
#include "ADN_Tr.h"
#include "ENT/ENT_Tr.h"

typedef helpers::AttritionEffectOnHuman AttritionEffectOnHuman;

//-----------------------------------------------------------------------------
// Internal table connector
//-----------------------------------------------------------------------------
class ADN_Connector_AttritionEffectTable
    : public ADN_Connector_Table_ABC
{
public:
    ADN_Connector_AttritionEffectTable( ADN_Categories_AttritionEffect_Table& tab )
        : ADN_Connector_Table_ABC( tab, false, "ADN_Connector_AttritionEffect_ListView" )
    {
        // NOTHING
    }
    void AddSubItems( int i, void* pObj )
    {
        assert( pObj );
        // Add a new row.
        ADN_TableItem_String* pItemState = new ADN_TableItem_String( &tab_, pObj );
        ADN_TableItem_Int* pItemWounded = new ADN_TableItem_Int( &tab_, pObj );
        ADN_TableItem_Int* pItemDead = new ADN_TableItem_Int( &tab_, pObj );
        tab_.setItem( i, 0, pItemState );
        tab_.setItem( i, 1, pItemWounded );
        tab_.setItem( i, 2, pItemDead );
        // Connect the items.
        pItemState->setText( ADN_Tr::ConvertFromEquipmentState( static_cast<AttritionEffectOnHuman*>(pObj)->nEquipmentState_.GetData(), ENT_Tr_ABC::eToTr ).c_str() );
        pItemWounded->GetConnector().Connect( &static_cast<AttritionEffectOnHuman*>(pObj)->nInjuredPercentage_ );
        pItemDead->GetConnector().Connect( &static_cast<AttritionEffectOnHuman*>(pObj)->nDeadPercentage_ );
        pItemWounded->GetValidator().setTop( 100 - pItemDead->text().toInt() );
        pItemDead->GetValidator().setTop( 100 - pItemWounded->text().toInt() );
    }
};

// -----------------------------------------------------------------------------
// Name: ADN_Categories_AttritionEffect_Table constructor
// Created: SBO 2006-07-28
// -----------------------------------------------------------------------------
ADN_Categories_AttritionEffect_Table::ADN_Categories_AttritionEffect_Table( QWidget* pParent /* = 0*/ )
    : ADN_Table2( pParent, "ADN_Categories_AttritionEffect_Table" )
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
    setNumCols( 3 );
    setNumRows( 0 );
    setColumnStretchable( 0, true );
    setColumnStretchable( 1, true );
    setColumnStretchable( 2, true );
    horizontalHeader()->setLabel( 0, tr( "State" ) );
    horizontalHeader()->setLabel( 1, tr( "Wounded %" ) );
    horizontalHeader()->setLabel( 2, tr( "Dead %" ) );
    setColumnReadOnly( 0, true );
    // Create the connector.
    pConnector_ = new ADN_Connector_AttritionEffectTable( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_AttritionEffect_Table destructor
// Created: SBO 2006-07-28
// -----------------------------------------------------------------------------
ADN_Categories_AttritionEffect_Table::~ADN_Categories_AttritionEffect_Table()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_AttritionEffect_Table::doValueChanged
// Created: JSR 2010-05-03
// -----------------------------------------------------------------------------
void ADN_Categories_AttritionEffect_Table::doValueChanged( int row, int col )
{
    if( col == 1 || col == 2 )
        UpdateValidators( row );
    ADN_Table2::doValueChanged( row, col );
    ADN_Workspace::GetWorkspace().GetEquipements().GetGui().UpdateGraph();
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_AttritionEffect_Table::UpdateValidators
// Created: LGY 2010-08-12
// -----------------------------------------------------------------------------
void ADN_Categories_AttritionEffect_Table::UpdateValidators( int row )
{
    ADN_TableItem_Int* pItemWounded = (ADN_TableItem_Int*)this->item( row, 1 );
    ADN_TableItem_Int* pItemDead = (ADN_TableItem_Int*)this->item( row, 2 );
    pItemWounded->GetValidator().setTop( 100 - pItemDead->text().toInt() );
    pItemDead->GetValidator().setTop( 100 - pItemWounded->text().toInt() );
}
