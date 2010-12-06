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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Table_Objects_FirePropagationModifier.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 17:04 $
// $Revision: 7 $
// $Workfile: ADN_Table_Objects_FirePropagationModifier.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Table_Objects_FirePropagationModifier.h"

#include "ADN_Objects_Data.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_TableItem_Edit.h"

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

    void  AddSubItems(int i,void *obj)
    {
        assert(obj);
        ADN_TableItem_String *pNameItem=0;
        ADN_TableItem_Int *pIgnitionThresholdItem=0;
        ADN_TableItem_Int *pMaxCombustionEnergyItem=0;

        // add a new row & set new values
        tab_.setItem(i,0,pNameItem=new ADN_TableItem_String(&tab_,obj));
        tab_.setItem(i,1,pIgnitionThresholdItem=new ADN_TableItem_Int(&tab_,obj));
        tab_.setItem(i,2,pMaxCombustionEnergyItem=new ADN_TableItem_Int(&tab_,obj));

        // disable first column
        pNameItem->setEnabled(false);

        // connect items & datas
        ADN_Objects_Data::ADN_CapacityInfos_FirePropagationModifier::ModifierByFireClass* pModifier = static_cast< ADN_Objects_Data::ADN_CapacityInfos_FirePropagationModifier::ModifierByFireClass* >( obj );
        pNameItem->GetConnector().Connect(&pModifier->ptrFireClass_.GetData()->strName_ );
        pIgnitionThresholdItem->GetConnector().Connect( &pModifier->ignitionThreshold_ );
        pMaxCombustionEnergyItem->GetConnector().Connect( &pModifier->maxCombustionEnergy_ );
    }
};

//-----------------------------------------------------------------------------
// Name: ADN_Table_Objects_FirePropagationModifier constructor
// Created: SLG 2010-04-13
//-----------------------------------------------------------------------------
ADN_Table_Objects_FirePropagationModifier::ADN_Table_Objects_FirePropagationModifier( QWidget* pParent )
: ADN_Table2( pParent, "ADN_Table_Objects_FirePropagationModifier" )
{
    // peut etre selectionne & trie
    setSorting(true);
    setSelectionMode(QTable::NoSelection);
    setShowGrid(false);
    setLeftMargin(0);

    // hide vertical header
    verticalHeader()->hide();

    // tab with 2 columns
    setNumCols(3);
    setNumRows(0);
    setColumnStretchable(0,true);
    setColumnStretchable(1,true);

    horizontalHeader()->setLabel(0, tr( "Fire class" ) );
    horizontalHeader()->setLabel(1, tr( "Ignition threshold" ) );
    horizontalHeader()->setLabel(2, tr( "Max combustion energy" ) );

    // connector creation
    pConnector_=new ADN_Table_Objects_FirePropagationModifier_Connector(*this);
}


//-----------------------------------------------------------------------------
// Name: ADN_Table_Objects_FirePropagationModifier destructor
// Created: SLG 2010-04-13
//-----------------------------------------------------------------------------
ADN_Table_Objects_FirePropagationModifier::~ADN_Table_Objects_FirePropagationModifier()
{
    delete pConnector_;
}

