// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Urban_AttritionTable.h"
#include "ADN_Urban_Data.h"
#include "ADN_Urban_GUI.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_TableItem_Edit.h"

//-----------------------------------------------------------------------------
// Internal Table connector to be connected with
//-----------------------------------------------------------------------------
class ADN_Urban_AttritionTable_Connector : public ADN_Connector_Table_ABC
{

public:
    explicit ADN_Urban_AttritionTable_Connector( ADN_Urban_AttritionTable& tab )
        : ADN_Connector_Table_ABC( tab, false )
    {}

    void  AddSubItems( int i, void* pObj )
    {
        assert( pObj != 0 );
        helpers::AttritionInfos* pAttrition = ( helpers::AttritionInfos* )pObj;

        tab_.verticalHeader()->setLabel( i, pAttrition->ptrArmor_.GetData()->strName_.GetData().c_str() );

        // add a new row & set new values
        ADN_TableItem_DoublePercentage* pItem0 = new ADN_TableItem_DoublePercentage( &tab_, pObj );
        pItem0->UseColor( true );
        pItem0->SetRangeForColor( 0.0, 100.0 );
        tab_.setItem( i, 0, pItem0 );
        static_cast< ADN_DoublePercentageValidator* >( &pItem0->GetValidator() )->AddLinkedValue( pAttrition->rRepairWithEvac_ );
        static_cast< ADN_DoublePercentageValidator* >( &pItem0->GetValidator() )->AddLinkedValue( pAttrition->rRepairNoEvac_ );
        pItem0->GetConnector().Connect( & pAttrition->rDestroy_ );

        ADN_TableItem_DoublePercentage* pItem1 = new ADN_TableItem_DoublePercentage( &tab_, pObj );
        pItem1->UseColor( true );
        pItem1->SetRangeForColor( 0.0, 100.0 );
        tab_.setItem( i, 1, pItem1 );
        static_cast< ADN_DoublePercentageValidator* >( &pItem1->GetValidator() )->AddLinkedValue( pAttrition->rDestroy_ );
        static_cast< ADN_DoublePercentageValidator* >( &pItem1->GetValidator() )->AddLinkedValue( pAttrition->rRepairNoEvac_ );
        pItem1->GetConnector().Connect( & pAttrition->rRepairWithEvac_ );

        ADN_TableItem_DoublePercentage* pItem2 = new ADN_TableItem_DoublePercentage( &tab_, pObj );
        pItem2->UseColor( true );
        pItem2->SetRangeForColor( 0.0, 100.0 );
        tab_.setItem( i, 2, pItem2 );
        static_cast< ADN_DoublePercentageValidator* >( &pItem2->GetValidator() )->AddLinkedValue( pAttrition->rDestroy_ );
        static_cast< ADN_DoublePercentageValidator* >( &pItem2->GetValidator() )->AddLinkedValue( pAttrition->rRepairWithEvac_ );
        pItem2->GetConnector().Connect( & pAttrition->rRepairNoEvac_ );
    }
};

//-----------------------------------------------------------------------------
// Name: ADN_Urban_AttritionTable constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Urban_AttritionTable::ADN_Urban_AttritionTable( QWidget* pParent )
: ADN_Table2( pParent, "ADN_Urban_AttritionTable" )
{
    // Selection and style.
    //$$$$ Find a better policy
    setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum );
    setMinimumHeight( 160 );
    setSelectionMode( Q3Table::NoSelection );
    setShowGrid( false );

    // Set up the columns.
    setNumCols(3);
    horizontalHeader()->setLabel( 0, tr( "% destroyed" ) );
    horizontalHeader()->setLabel( 1, tr( "% maintenance support needed" ) );
    horizontalHeader()->setLabel( 2, tr( "% on site fixable" ) );
    setColumnStretchable( 0, true );
    setColumnStretchable( 1, true );
    setColumnStretchable( 2, true );

    // Connector creation.
    pConnector_ = new ADN_Urban_AttritionTable_Connector( *this );
}


//-----------------------------------------------------------------------------
// Name: ADN_Urban_AttritionTable destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Urban_AttritionTable::~ADN_Urban_AttritionTable()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_AttritionTable::doValueChanged
// Created: JSR 2010-04-30
// -----------------------------------------------------------------------------
void ADN_Urban_AttritionTable::doValueChanged( int row, int col )
{
    ADN_Table2::doValueChanged( row, col );
}
