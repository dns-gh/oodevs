// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_MissionParameterValues_Table.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_Missions_Data.h"
#include "ADN_CommonGfx.h"
#include <Qt3Support/q3popupmenu.h>

namespace
{
    class ADN_CT_MissionParameterValues : public ADN_Connector_Table_ABC
    {
    public:
        ADN_CT_MissionParameterValues( ADN_MissionParameterValues_Table& tab )
            : ADN_Connector_Table_ABC( tab, false )
        {
            SetAutoClear( true );
        }

        void AddSubItems( int i, void* obj )
        {
            assert( obj );

            ADN_Missions_Data::MissionParameterValue* param = static_cast< ADN_Missions_Data::MissionParameterValue* >( obj );
            ADN_TableItem_String* itemName = new ADN_TableItem_String( &tab_, obj );
            tab_.setItem( i, 0, itemName );
            itemName->GetConnector().Connect( &param->name_ );
            tab_.show();
        }
    };
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameterValues_Table constructor
// Created: SBO 2006-12-05
// -----------------------------------------------------------------------------
ADN_MissionParameterValues_Table::ADN_MissionParameterValues_Table( QWidget* pParent, const char* szName )
    : ADN_Table2( pParent, szName )
{
    verticalHeader()->hide();
    setLeftMargin( 0 );
    setNumCols( 1 );
    setNumRows( 0 );
    horizontalHeader()->setLabel( 0, tr( "Name" ) );
    pConnector_ = new ADN_CT_MissionParameterValues( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameterValues_Table destructor
// Created: SBO 2006-12-05
// -----------------------------------------------------------------------------
ADN_MissionParameterValues_Table::~ADN_MissionParameterValues_Table()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameterValues_Table::AddNewElement
// Created: SBO 2006-12-05
// -----------------------------------------------------------------------------
void ADN_MissionParameterValues_Table::AddNewElement()
{
    ADN_Missions_Data::MissionParameterValue* newElement = new ADN_Missions_Data::MissionParameterValue();
    newElement->name_ = tr( "New value" ).ascii();

    ADN_Connector_Vector_ABC* pCTable = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
    pCTable->AddItem( newElement );
    pCTable->AddItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameterValues_Table::RemoveCurrentElement
// Created: SBO 2006-12-05
// -----------------------------------------------------------------------------
void ADN_MissionParameterValues_Table::RemoveCurrentElement()
{
    ADN_Missions_Data::MissionParameterValue* param = (ADN_Missions_Data::MissionParameterValue*)GetCurrentData();
    if( param )
        static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( param );
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameterValues_Table::OnContextMenu
// Created: SBO 2006-12-05
// -----------------------------------------------------------------------------
void ADN_MissionParameterValues_Table::OnContextMenu( int /*row*/, int /*col*/, const QPoint& pt )
{
    Q3PopupMenu popup( this );

    popup.insertItem( tr( "Add value"), 0 );
    if( GetCurrentData() != 0 )
        popup.insertItem( tr( "Remove value"), 1 );

    int result = popup.exec( pt );
    if( result == 1 )
        RemoveCurrentElement();
    else if( result == 0 )
        AddNewElement();
}
