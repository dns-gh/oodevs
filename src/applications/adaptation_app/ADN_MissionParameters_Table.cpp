// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_MissionParameters_Table.h"
#include "moc_ADN_MissionParameters_Table.cpp"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_Missions_Data.h"
#include "ADN_TableItem_CheckItem.h"
#include "ADN_MissionParameterType.h"
#include "ADN_Missions_GUI.h"
#include "ADN_Tr.h"
#include <qpopupmenu.h>

namespace
{
    class ADN_CT_MissionParameters : public ADN_Connector_Table_ABC
    {
    public:
        ADN_CT_MissionParameters( ADN_MissionParameters_Table& tab, T_ConnectorVector& itemConnectors )
            : ADN_Connector_Table_ABC( tab, false ), itemConnectors_( itemConnectors ) {}

        void AddSubItems( int i, void* obj )
        {
            assert( obj );

            ADN_Missions_Data::MissionParameter* param = static_cast< ADN_Missions_Data::MissionParameter* >( obj );

            ADN_TableItem_String*     itemName     = new ADN_TableItem_String    ( &tab_, obj );
            ADN_MissionParameterType* itemType     = new ADN_MissionParameterType( &tab_, obj, itemConnectors_ );
            ADN_TableItem_CheckItem*  itemOptional = new ADN_TableItem_CheckItem ( &tab_, obj );

            tab_.setItem( i, 0, itemName );
            tab_.setItem( i, 1, itemType );
            tab_.setItem( i, 2, itemOptional );

            itemName    ->GetConnector().Connect( &param->strName_ );
            itemType    ->GetConnector().Connect( &param->type_ );
            itemOptional->GetConnector().Connect( &param->isOptional_ );
        }

    private:
        T_ConnectorVector& itemConnectors_;
    };
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameters_Table constructor
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_MissionParameters_Table::ADN_MissionParameters_Table( QWidget* parent /*= 0*/ )
    : ADN_Table2( parent, "ADN_MissionParameters_Table" )
{
    verticalHeader()->hide();
    setLeftMargin( 0 );
    setNumCols( 3 );
    setNumRows( 0 );
    horizontalHeader()->setLabel( 0, tr( "Name" ) );
    horizontalHeader()->setLabel( 1, tr( "Type" ) );
    horizontalHeader()->setLabel( 2, tr( "Optional" ) );
    pConnector_ = new ADN_CT_MissionParameters( *this, itemConnectors_ );

    connect( this, SIGNAL( selectionChanged() ), this, SLOT( OnSelectionChanged() ) );
}
    
// -----------------------------------------------------------------------------
// Name: ADN_MissionParameters_Table destructor
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_MissionParameters_Table::~ADN_MissionParameters_Table()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameters_Table::OnContextMenu
// Created: SBO 2006-12-05
// -----------------------------------------------------------------------------
void ADN_MissionParameters_Table::OnContextMenu( int /*row*/, int /*col*/, const QPoint& pt )
{
    QPopupMenu popup( this );

    popup.insertItem( tr( "Add parameter"), 0 );
    if ( GetCurrentData() != 0 )
        popup.insertItem( tr( "Remove parameter"), 1 );
    
    int result = popup.exec( pt );
    if( result == 1 )
        RemoveCurrentElement();
    else if( result == 0 )
        AddNewElement();
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameters_Table::AddNewElement
// Created: SBO 2006-12-05
// -----------------------------------------------------------------------------
void ADN_MissionParameters_Table::AddNewElement()
{
    ADN_Missions_Data::MissionParameter* newElement = new ADN_Missions_Data::MissionParameter();
    newElement->strName_ = tr( "New parameter" ).ascii();

    ADN_Connector_Vector_ABC* pCTable = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
    pCTable->AddItem( newElement );
    pCTable->AddItem( 0 ); 
}
    
// -----------------------------------------------------------------------------
// Name: ADN_MissionParameters_Table::RemoveCurrentElement
// Created: SBO 2006-12-05
// -----------------------------------------------------------------------------
void ADN_MissionParameters_Table::RemoveCurrentElement()
{
    ADN_Missions_Data::MissionParameter* param = (ADN_Missions_Data::MissionParameter*)GetCurrentData();
    if ( param )
        static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( param );
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameters_Table::SetItemConnector
// Created: SBO 2006-12-05
// -----------------------------------------------------------------------------
void ADN_MissionParameters_Table::SetItemConnectors( const T_ConnectorVector& itemConnectors  )
{
    itemConnectors_ = itemConnectors;
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameters_Table::OnSelectionChanged
// Created: SBO 2006-12-06
// -----------------------------------------------------------------------------
void ADN_MissionParameters_Table::OnSelectionChanged()
{
    ADN_MissionParameterType* current = static_cast< ADN_MissionParameterType* >( item( currentRow(), 1 ) );
    if( current )
        current->Update();
}

