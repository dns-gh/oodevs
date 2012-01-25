// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Equipement_AttritionTable.h"
#include "ADN_Equipement_Data.h"
#include "ADN_Equipement_GUI.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_TableItem_Edit.h"
#include "ADN_AttritionInfos.h"
#include <boost/noncopyable.hpp>

typedef helpers::AttritionInfos AttritionInfos;

//-----------------------------------------------------------------------------
// Internal Table connector to be connected with
//-----------------------------------------------------------------------------
class ADN_Equipement_AttritionTable_Connector : public ADN_Connector_Table_ABC
{

public:
    explicit ADN_Equipement_AttritionTable_Connector( ADN_Equipement_AttritionTable& tab )
        : ADN_Connector_Table_ABC( tab, false )
    {}

    void  AddSubItems( int i, void* pObj )
    {
        assert( pObj != 0 );
        AttritionInfos* pAttrition = (AttritionInfos*)pObj;

        tab_.verticalHeader()->setLabel( i, pAttrition->ptrArmor_.GetData()->strName_.GetData().c_str() );

        // add a new row & set new values
        ADN_TableItem_DoublePercentage* pItem0 = new ADN_TableItem_DoublePercentage( &tab_, pObj );
        pItem0->UseColor( true );
        tab_.setItem( i, 0, pItem0 );
        static_cast< ADN_DoublePercentageValidator* >( &pItem0->GetValidator() )->AddLinkedValue( pAttrition->rRepairWithEvac_ );
        static_cast< ADN_DoublePercentageValidator* >( &pItem0->GetValidator() )->AddLinkedValue( pAttrition->rDestroy_ );
        pItem0->GetConnector().Connect( & pAttrition->rRepairNoEvac_ );

        ADN_TableItem_DoublePercentage* pItem1 = new ADN_TableItem_DoublePercentage( &tab_, pObj );
        pItem1->UseColor( true );
        tab_.setItem( i, 1, pItem1 );
        static_cast< ADN_DoublePercentageValidator* >( &pItem1->GetValidator() )->AddLinkedValue( pAttrition->rDestroy_ );
        static_cast< ADN_DoublePercentageValidator* >( &pItem1->GetValidator() )->AddLinkedValue( pAttrition->rRepairNoEvac_ );
        pItem1->GetConnector().Connect( & pAttrition->rRepairWithEvac_ );

        ADN_TableItem_DoublePercentage* pItem2 = new ADN_TableItem_DoublePercentage( &tab_, pObj );
        pItem2->UseColor( true );
        tab_.setItem( i, 2, pItem2 );
        static_cast< ADN_DoublePercentageValidator* >( &pItem2->GetValidator() )->AddLinkedValue( pAttrition->rRepairNoEvac_ );
        static_cast< ADN_DoublePercentageValidator* >( &pItem2->GetValidator() )->AddLinkedValue( pAttrition->rRepairWithEvac_ );
        pItem2->GetConnector().Connect( & pAttrition->rDestroy_ );
    }
};



//-----------------------------------------------------------------------------
// Name: ADN_Equipement_AttritionTable constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Equipement_AttritionTable::ADN_Equipement_AttritionTable( QWidget* pParent )
: ADN_Table2( pParent, "ADN_Equipement_AttritionTable" )
{
    // Selection and style.
    //$$$$ Find a better policy
    setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum );
    setMinimumHeight( 160 );
    setSelectionMode( Q3Table::NoSelection );
    setShowGrid( false );

    // Set up the columns.
    setNumCols(3);
    horizontalHeader()->setLabel( 0, tr( "% on site fixable" ) );
    horizontalHeader()->setLabel( 1, tr( "% maintenance support needed" ) );
    horizontalHeader()->setLabel( 2, tr( "% destroyed" ) );
    setColumnStretchable( 0, true );
    setColumnStretchable( 1, true );
    setColumnStretchable( 2, true );

    // Connector creation.
    pConnector_ = new ADN_Equipement_AttritionTable_Connector( *this );
}


//-----------------------------------------------------------------------------
// Name: ADN_Equipement_AttritionTable destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Equipement_AttritionTable::~ADN_Equipement_AttritionTable()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_AttritionTable::doValueChanged
// Created: JSR 2010-04-30
// -----------------------------------------------------------------------------
void ADN_Equipement_AttritionTable::doValueChanged( int row, int col )
{
    ADN_Table2::doValueChanged( row, col );
    ADN_Workspace::GetWorkspace().GetEquipements().GetGui().UpdateGraph();
}
