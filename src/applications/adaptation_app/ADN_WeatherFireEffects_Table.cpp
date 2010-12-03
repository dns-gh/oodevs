// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-12-29 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_WeatherFireEffects_Table.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 17:04 $
// $Revision: 7 $
// $Workfile: ADN_WeatherFireEffects_Table.cpp $
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
        ADN_TableItem_String *pItemString=0;
        ADN_TableItem_Int *pItemInt=0;

        // set new values
        tab_.setItem(i,0,pItemString=new ADN_TableItem_String(&tab_,obj));
        tab_.setItem(i,1,pItemInt=new ADN_TableItem_Int(&tab_,obj));

        // disable first column
        pItemString->setEnabled(false);
        pItemString->setText( ADN_Tr::ConvertFromSensorWeatherModifiers(static_cast<ADN_WeatherFireEffects*>(obj)->weatherType_,ENT_Tr_ABC::eToTr).c_str() );

        // set table item properties
        //pItemInt->GetValidator().setRange( 0, 1, 2 );

        // connect items & datas
        pItemInt->GetConnector().Connect( &static_cast<ADN_WeatherFireEffects*>(obj)->heatDecreaseRate_ );
    }
};



//-----------------------------------------------------------------------------
// Name: ADN_WeatherFireEffects_Table constructor
// Created: SLG 2010-04-13
//-----------------------------------------------------------------------------
ADN_WeatherFireEffects_Table::ADN_WeatherFireEffects_Table( QWidget* pParent )
: ADN_Table2( pParent, "ADN_WeatherFireEffects_Table" )
{
    // peut etre selectionne & trie
    setSorting(true);
    setSelectionMode(QTable::NoSelection);
    setShowGrid(false);
    setLeftMargin(0);

    // hide vertical header
    verticalHeader()->hide();

    // tab with 2 columns
    setNumCols(2);
    setNumRows(0);
    setColumnStretchable(0,true);
    setColumnStretchable(1,true);

    horizontalHeader()->setLabel(0, tr( "Weather" ) );
    horizontalHeader()->setLabel(1, tr( "Heat decrease rate" ) );

    // connector creation
    pConnector_=new ADN_WeatherFireEffects_Table_Connector(*this);
}


//-----------------------------------------------------------------------------
// Name: ADN_WeatherFireEffects_Table destructor
// Created: SLG 2010-04-13
//-----------------------------------------------------------------------------
ADN_WeatherFireEffects_Table::~ADN_WeatherFireEffects_Table()
{
    delete pConnector_;
}

