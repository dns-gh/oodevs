// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_MissionTypes_Table.h"
#include "ADN_Missions_Data.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_TableItem_Edit.h"
#include "ADN_TableItem_CheckItem.h"

//-----------------------------------------------------------------------------
// Internal Table connector to be connected with T_ModificatorIlluminationInfos_Vector
//-----------------------------------------------------------------------------
class ADN_CT_MissionTypes
:public ADN_Connector_Table_ABC
{
public:
    ADN_CT_MissionTypes(ADN_MissionTypes_Table& tab)
    : ADN_Connector_Table_ABC(tab,false)
    {}

    void AddSubItems(int i,void* obj)
    {
        ADN_Missions_Data::MissionType* param = static_cast< ADN_Missions_Data::MissionType* >( obj );

        Q3TableItem* itemName = new Q3TableItem( &tab_, Q3TableItem::Never );
        ADN_TableItem_CheckItem*  itemAllowed = new ADN_TableItem_CheckItem ( &tab_, obj );

        tab_.setItem( i, 0, itemName );
        tab_.setItem( i, 1, itemAllowed );

        itemName->setText( param->displayName_.c_str() );
        itemAllowed->GetConnector().Connect( &param->isAllowed_ );
    }

private:
    ADN_CT_MissionTypes& operator=( const ADN_CT_MissionTypes& );
};

// -----------------------------------------------------------------------------
// Name: ADN_MissionTypes_Table constructor
// Created: LDC 2010-08-19
// -----------------------------------------------------------------------------
ADN_MissionTypes_Table::ADN_MissionTypes_Table( QWidget* pParent, const char* szName )
: ADN_Table2( pParent, szName  )
{
    // peut etre selectionne & trie
    setSorting(true);
    setSelectionMode(Q3Table::NoSelection);
    setShowGrid(false);
    setLeftMargin(0);

    // hide vertical header
    verticalHeader()->hide();

    // tab with 2 columns
    setNumCols(2);
    setNumRows(0);
    setColumnStretchable(0,true);
    setColumnStretchable(1,true);
    setMaximumWidth( 300 );

    horizontalHeader()->setLabel(0, qApp->translate( "ADN_Missions_GUI", "Type" ) );
    horizontalHeader()->setLabel(1, qApp->translate( "ADN_Missions_GUI", "Allowed" ) );

    // connector creation
    pConnector_=new ADN_CT_MissionTypes(*this);
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionTypes_Table destructor
// Created: LDC 2010-08-19
// -----------------------------------------------------------------------------
ADN_MissionTypes_Table::~ADN_MissionTypes_Table()
{
    // NOTHING
}

