// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-02-10 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_Automata_SubUnitsTable.cpp $
// $Author: Nld $
// $Modtime: 9/05/05 12:13 $
// $Revision: 10 $
// $Workfile: ADN_Automata_SubUnitsTable.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Automata_SubUnitsTable.h"
#include "moc_ADN_Automata_SubUnitsTable.cpp"

#include <qpopupmenu.h>
#include <qapplication.h>
#include <qeventloop.h>

#include "ADN_Automata_Data.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_TableItem_Edit.h"
#include "ADN_Validator.h"


typedef ADN_Automata_Data::UnitInfos UnitInfos;


//-----------------------------------------------------------------------------
// Internal table connector
//-----------------------------------------------------------------------------
class ADN_Connector_SubUnitTable
    : public ADN_Connector_Table_ABC
{
public:
    ADN_Connector_SubUnitTable( ADN_Automata_SubUnitsTable& tab ) 
        : ADN_Connector_Table_ABC( tab, false, "ADN_Automata_SubUnitsTable" )
    {}

    void AddSubItems( int i, void* pObj )
    {
        assert( pObj );
        ADN_TableItem_String* pItemName = new ADN_TableItem_String( &tab_, pObj, QTableItem::Never );
        ADN_TableItem_String* pItemCountRegExp = new ADN_TableItem_String( &tab_, pObj );
        pItemCountRegExp->SetAlignment( Qt::AlignRight );
        // Add a new row.
        tab_.setItem( i, 0, pItemName );
        tab_.setItem( i, 1, pItemCountRegExp ); 


        // connect items & datas
        pItemName->GetConnector().Connect( &static_cast<UnitInfos*>(pObj)->ptrUnit_.GetData()->strName_ );
        pItemCountRegExp->GetConnector().Connect( &static_cast<UnitInfos*>(pObj)->strNbrRegExp_ );
    }
};


// -----------------------------------------------------------------------------
// Name: ADN_Automata_SubUnitsTable constructor
// Created: APE 2005-01-07
// -----------------------------------------------------------------------------
ADN_Automata_SubUnitsTable::ADN_Automata_SubUnitsTable( QWidget* pParent )
: ADN_Table2( pParent, "ADN_Automata_SubUnitsTable" )
, bMenuListItemSelected_( false )
{
    // Selection and sorting.
    setSorting( true );
    setSelectionMode( QTable::NoSelection );
    setShowGrid( false );
    setLeftMargin( 0 );

    this->setMaximumHeight( 300 );

    // Hide the vertical header.
    verticalHeader()->hide();

    // Setup 2 columns.
    setNumCols( 2 );
    setNumRows( 0 );
    setColumnStretchable( 0, true );
    setColumnStretchable( 1, true );

    horizontalHeader()->setLabel(0, tr( "Name" ) );
    horizontalHeader()->setLabel(1, tr( "Nbr" ) );

    // Create the connector.
    pConnector_ = new ADN_Connector_SubUnitTable( *this );
}


// -----------------------------------------------------------------------------
// Name: ADN_Automata_SubUnitsTable destructor
// Created: APE 2005-01-07
// -----------------------------------------------------------------------------
ADN_Automata_SubUnitsTable::~ADN_Automata_SubUnitsTable()
{
    delete pConnector_;
}


// -----------------------------------------------------------------------------
// Name: ADN_Automata_SubUnitsTable::OnContextMenu
// Created: APE 2005-01-10
// -----------------------------------------------------------------------------
void ADN_Automata_SubUnitsTable::OnContextMenu( int /*nRow*/, int /*nCol*/, const QPoint& pt )
{
    QPopupMenu popupMenu( this );
    QPopupMenu& addMenu = *new QPopupMenu( &popupMenu );

    // Get the list of the possible units
    ADN_Units_Data::T_UnitInfos_Vector& units = ADN_Workspace::GetWorkspace().GetUnits().GetData().GetUnitsInfos();

    QListView& listView = *new QListView( &addMenu );
    listView.addColumn( "" );
    listView.header()->hide();
    listView.setAllColumnsShowFocus( true );
    listView.setResizeMode( QListView::AllColumns );
    listView.setMinimumSize( 200, std::min( 800, (int)units.size() * 17 ) );

    for( ADN_Units_Data::IT_UnitInfos_Vector it = units.begin(); it != units.end(); ++it )
    {
        new MT_ValuedListViewItem<int>( (int)(*it), &listView, (*it)->strName_.GetData().c_str() );
    }

    addMenu.insertItem( &listView );

    popupMenu.insertItem( tr( "Add sub-unit"), &addMenu );
    popupMenu.insertItem( tr( "Remove sub-unit"), 0 );
    popupMenu.setItemEnabled( 0, GetCurrentData() != 0 );

    bMenuListItemSelected_ = false;
    connect( &listView, SIGNAL( doubleClicked( QListViewItem*, const QPoint&, int ) ), this, SLOT( MenuListItemSelected() ) );
    connect( &listView, SIGNAL( returnPressed( QListViewItem* ) ), this, SLOT( MenuListItemSelected() ) );
    int nMenuResult = popupMenu.exec(pt);
    if( nMenuResult == 0 )
    {
        RemoveCurrentElement();
    }
    else if(  bMenuListItemSelected_ )
    {
        assert( listView.selectedItem() != 0 );
        MT_ValuedListViewItem<int>* pItem = (MT_ValuedListViewItem<int>*)listView.selectedItem();
        AddNewElement( pItem->GetValue() );
    }
}


// -----------------------------------------------------------------------------
// Name: ADN_Automata_SubUnitsTable::AddNewElement
// Created: APE 2005-05-09
// -----------------------------------------------------------------------------
void ADN_Automata_SubUnitsTable::AddNewElement( int n )
    {
        // Create a new element
        UnitInfos* pNewInfo = new UnitInfos();
    pNewInfo->ptrUnit_ = (ADN_Units_Data::UnitInfos*)n;
        ADN_Connector_Vector_ABC* pCTable = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
        pCTable->AddItem( pNewInfo );
        pCTable->AddItem( 0 );  
    }


// -----------------------------------------------------------------------------
// Name: ADN_Automata_SubUnitsTable::RemoveCurrentElement
// Created: APE 2005-05-09
// -----------------------------------------------------------------------------
void ADN_Automata_SubUnitsTable::RemoveCurrentElement()
{
    // Delete the current element.
    UnitInfos* pCurPh = (UnitInfos*)GetCurrentData();
    if( pCurPh != 0 )
        static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( pCurPh );
}


// -----------------------------------------------------------------------------
// Name: ADN_Automata_SubUnitsTable::MenuListItemSelected
// Created: APE 2005-04-15
// -----------------------------------------------------------------------------
void ADN_Automata_SubUnitsTable::MenuListItemSelected()
{
    bMenuListItemSelected_ = true;
    qApp->eventLoop()->exitLoop();
}