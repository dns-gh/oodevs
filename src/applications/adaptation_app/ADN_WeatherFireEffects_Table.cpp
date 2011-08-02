// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_WeatherFireEffects_Table.h"
#include "ADN_WeatherFireEffects.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_TableItem_Edit.h"
#include "ADN_Tr.h"

//-----------------------------------------------------------------------------
// Internal Table connector to be connected with
//-----------------------------------------------------------------------------
class ADN_WeatherFireEffects_Table_Connector
    : public ADN_Connector_Table_ABC
{
public:
    ADN_WeatherFireEffects_Table_Connector( ADN_WeatherFireEffects_Table& tab )
        : ADN_Connector_Table_ABC( tab, false )
    {}

    void  AddSubItems( int i, void *obj )
    {
        assert(obj);
        ADN_TableItem_String *pItemString = new ADN_TableItem_String( &tab_, obj );
        ADN_TableItem_Int *pItemInt = new ADN_TableItem_Int( &tab_, obj );
        // set new values
        tab_.setItem( i, 0, pItemString );
        tab_.setItem( i, 1, pItemInt );
        // disable first column
        pItemString->setEnabled(false);
        pItemString->setText( ADN_Tr::ConvertFromSensorWeatherModifiers( static_cast< ADN_WeatherFireEffects* >( obj )->weatherType_, ENT_Tr_ABC::eToTr ).c_str() );
        // connect items & datas
        pItemInt->GetConnector().Connect( &static_cast< ADN_WeatherFireEffects* >( obj )->heatDecreaseRate_ );
    }
};

//-----------------------------------------------------------------------------
// Name: ADN_WeatherFireEffects_Table constructor
// Created: BCI 2010-12-02
//-----------------------------------------------------------------------------
ADN_WeatherFireEffects_Table::ADN_WeatherFireEffects_Table( QWidget* pParent )
    : ADN_Table2( pParent, "ADN_WeatherFireEffects_Table" )
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
    horizontalHeader()->setLabel( 0, tr( "Weather" ) );
    horizontalHeader()->setLabel( 1, tr( "Heat decrease rate" ) );
    // connector creation
    pConnector_ = new ADN_WeatherFireEffects_Table_Connector( *this );
}

//-----------------------------------------------------------------------------
// Name: ADN_WeatherFireEffects_Table destructor
// Created: BCI 2010-12-02
//-----------------------------------------------------------------------------
ADN_WeatherFireEffects_Table::~ADN_WeatherFireEffects_Table()
{
    delete pConnector_;
}

