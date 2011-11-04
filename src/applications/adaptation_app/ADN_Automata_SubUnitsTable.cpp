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
#include "ADN_Automata_Data.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_TableItem_Edit.h"
#include "ADN_Validator.h"
#include "MT_ValuedListViewItem.h"

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
        , tab_( tab )
    {}

    void AddSubItems( int i, void* pObj )
    {
        assert( pObj );
        ADN_TableItem_String* pItemName  = new ADN_TableItem_String( &tab_, pObj, Q3TableItem::Never );
        ADN_TableItem_Int* pItemMinCount = new ADN_TableItem_Int( &tab_, pObj );
        ADN_TableItem_Int* pItemMaxCount = new ADN_TableItem_Int( &tab_, pObj );
        // Add a new row.
        tab_.setItem( i, 0, pItemName );
        tab_.setItem( i, 1, pItemMinCount );
        tab_.setItem( i, 2, pItemMaxCount );

        // connect items & datas
        pItemName->GetConnector().Connect( &static_cast<UnitInfos*>(pObj)->ptrUnit_.GetData()->strName_ );
        pItemMinCount->GetConnector().Connect( &static_cast<UnitInfos*>(pObj)->min_ );
        pItemMaxCount->GetConnector().Connect( &static_cast<UnitInfos*>(pObj)->max_ );
        tab_.AddSubItems( static_cast<UnitInfos*>(pObj)->ptrUnit_.GetData()->strName_.GetData() );
    }
private:
    ADN_Automata_SubUnitsTable& tab_;
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
    setSelectionMode( Q3Table::NoSelection );
    setShowGrid( false );
    setLeftMargin( 0 );

    this->setMaximumHeight( 300 );

    // Hide the vertical header.
    verticalHeader()->hide();

    // Setup 3 columns.
    setNumCols( 3 );
    setNumRows( 0 );
    setColumnStretchable( 0, true );
    setColumnStretchable( 1, true );
    setColumnStretchable( 2, true );

    horizontalHeader()->setLabel( 0, tr( "Name" ) );
    horizontalHeader()->setLabel( 1, tr( "Min Nbr" ) );
    horizontalHeader()->setLabel( 2, tr( "Max Nbr" ) );

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

namespace
{
    class MenuListView : public QWidgetAction
    {
    public:
        explicit MenuListView( ADN_Automata_SubUnitsTable* receiver, QObject* parent )
            : QWidgetAction( parent )
            , receiver_( receiver )
            , listView_( 0 )
        {}
        virtual ~MenuListView() {}
        virtual QWidget* createWidget( QWidget* parent )
        {
            ADN_Units_Data::T_UnitInfos_Vector& units = ADN_Workspace::GetWorkspace().GetUnits().GetData().GetUnitsInfos();
            listView_ = new Q3ListView( parent );
            listView_->addColumn( "" );
            listView_->header()->hide();
            listView_->setAllColumnsShowFocus( true );
            listView_->setResizeMode( Q3ListView::AllColumns );
            listView_->setMinimumSize( 200, std::min( 500, (int)units.size() * 17 ) );
            for( ADN_Units_Data::IT_UnitInfos_Vector it = units.begin(); it != units.end(); ++it )
                new MT_ValuedListViewItem<int>( (int)(*it), listView_, (*it)->strName_.GetData().c_str() );
            connect( listView_, SIGNAL( doubleClicked( Q3ListViewItem*, const QPoint&, int ) ), receiver_, SLOT( MenuListItemSelected() ) );
            connect( listView_, SIGNAL( returnPressed( Q3ListViewItem* ) ), receiver_, SLOT( MenuListItemSelected() ) );
            return listView_;
        }
        int SelectedValue() const
        {
            if( listView_ )
            {
                MT_ValuedListViewItem< int >* pItem = (MT_ValuedListViewItem< int >*)listView_->selectedItem();
                return pItem->GetValue();
            }
            return -1;
        }
        ADN_Automata_SubUnitsTable* receiver_;
        Q3ListView* listView_;
    };
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_SubUnitsTable::OnContextMenu
// Created: APE 2005-01-10
// -----------------------------------------------------------------------------
void ADN_Automata_SubUnitsTable::OnContextMenu( int /*nRow*/, int /*nCol*/, const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    Q3PopupMenu& addMenu = *new Q3PopupMenu( &popupMenu );

    // Get the list of the possible units
    MenuListView* list = new MenuListView( this, &addMenu );
    addMenu.addAction( list );

    popupMenu.insertItem( tr( "Add subordinate"), &addMenu );
    popupMenu.insertItem( tr( "Remove subordinate"), 0 );
    popupMenu.setItemEnabled( 0, GetCurrentData() != 0 );

    bMenuListItemSelected_ = false;
    int nMenuResult = popupMenu.exec(pt);
    if( nMenuResult == 0 )
        RemoveCurrentElement();
    else if( bMenuListItemSelected_ )
        AddNewElement( list ->SelectedValue() );
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
    {
        static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( pCurPh );
        emit RemoveItem( pCurPh->ptrUnit_.GetData()->strName_.GetData() );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_SubUnitsTable::MenuListItemSelected
// Created: APE 2005-04-15
// -----------------------------------------------------------------------------
void ADN_Automata_SubUnitsTable::MenuListItemSelected()
{
    bMenuListItemSelected_ = true;
    qApp->exit_loop();
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_SubUnitsTable::AddSubItems
// Created: LGY 2011-11-03
// -----------------------------------------------------------------------------
void ADN_Automata_SubUnitsTable::AddSubItems( const std::string& name )
{
    emit AddItem( name );
}
