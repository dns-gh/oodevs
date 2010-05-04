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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Equipement_UrbanAttritionTable.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 17:04 $
// $Revision: 7 $
// $Workfile: ADN_Equipement_UrbanAttritionTable.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Equipement_UrbanAttritionTable.h"

#include "ADN_Equipement_Data.h"
#include "ADN_Equipement_GUI.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_TableItem_Edit.h"

typedef ADN_Equipement_Data::UrbanAttritionInfos UrbanAttritionInfos;

//-----------------------------------------------------------------------------
// Internal Table connector to be connected with
//-----------------------------------------------------------------------------
class ADN_Equipement_UrbanAttritionTable_Connector
    : public ADN_Connector_Table_ABC
{
    MT_COPYNOTALLOWED( ADN_Equipement_UrbanAttritionTable_Connector )

public:
    ADN_Equipement_UrbanAttritionTable_Connector( ADN_Equipement_UrbanAttritionTable& tab ) 
        : ADN_Connector_Table_ABC( tab, false )
    {}

    void  AddSubItems(int i,void *obj)
    {
        assert(obj);
        ADN_TableItem_String *pItemString=0;
        ADN_TableItem_Double *pItemDouble=0;

        // add a new row & set new values
        tab_.setItem(i,0,pItemString=new ADN_TableItem_String(&tab_,obj));
        tab_.setItem(i,1,pItemDouble=new ADN_TableItem_Double(&tab_,obj));

        // disable first column
        pItemString->setEnabled(false);


        // set table item properties
        pItemDouble->GetValidator().setRange( 0, 1, 2 );

        // connect items & datas
        pItemString->GetConnector().Connect(static_cast<UrbanAttritionInfos*>(obj)->ptrMaterial_.GetData());
        pItemDouble->GetConnector().Connect(&static_cast<UrbanAttritionInfos*>(obj)->rCoeff_);
    }
};



//-----------------------------------------------------------------------------
// Name: ADN_Equipement_UrbanAttritionTable constructor
// Created: SLG 2010-04-13
//-----------------------------------------------------------------------------
ADN_Equipement_UrbanAttritionTable::ADN_Equipement_UrbanAttritionTable( QWidget* pParent )
: ADN_Table2( pParent, "ADN_Equipement_UrbanAttritionTable" )
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

    horizontalHeader()->setLabel(0, tr( "Material" ) );
    horizontalHeader()->setLabel(1, tr( "Modifiers" ) );

    // connector creation
    pConnector_=new ADN_Equipement_UrbanAttritionTable_Connector(*this);
}


//-----------------------------------------------------------------------------
// Name: ADN_Equipement_UrbanAttritionTable destructor
// Created: SLG 2010-04-13
//-----------------------------------------------------------------------------
ADN_Equipement_UrbanAttritionTable::~ADN_Equipement_UrbanAttritionTable()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_UrbanAttritionTable::doValueChanged
// Created: JSR 2010-05-03
// -----------------------------------------------------------------------------
void ADN_Equipement_UrbanAttritionTable::doValueChanged( int row, int col )
{
    ADN_Table2::doValueChanged( row, col );
    ADN_Workspace::GetWorkspace().GetEquipements().GetGui().UpdateGraph();
}
