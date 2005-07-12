//*****************************************************************************
//
// $Created: JDY 03-07-28 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_Units_Composantes_GUI.cpp $
// $Author: Nld $
// $Modtime: 29/04/05 16:14 $
// $Revision: 15 $
// $Workfile: ADN_Units_Composantes_GUI.cpp $
//
//*****************************************************************************
#include "ADN_Pch.h"
#include "ADN_Units_Composantes_GUI.h"
#include "moc_ADN_Units_Composantes_GUI.cpp"

#include "ADN_App.h"
#include "ADN_Tools.h"
#include "ADN_CommonGfx.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_Units_Data.h"
#include "ADN_Workspace.h"
#include "ADN_TableItem_CheckItem.h"

#include <MT/MT_Qt/MT_ValuedListViewItem.h>

#include <qpopmenu.h>
#include <qeventloop.h>

typedef ADN_Units_Data::ComposanteInfos ComposanteInfos;


//-----------------------------------------------------------------------------
// Internal Table connector to be connected with
//-----------------------------------------------------------------------------
class ADN_Units_Composantes_GUI_Connector
:public ADN_Connector_Table_ABC
{
public:

    ADN_Units_Composantes_GUI_Connector( ADN_Units_Composantes_GUI& tab ) 
    : ADN_Connector_Table_ABC( tab, false )
    {}

    void  AddSubItems( int nRow, void* pObj )
    {
        assert( pObj != 0 );
        ADN_TableItem_String*    pItemName = new ADN_TableItem_String( &tab_, pObj, QTableItem::Never );
        ADN_TableItem_Int*       pItemNbr = new ADN_TableItem_Int( &tab_, pObj );
        ADN_TableItem_CheckItem* pItemMajor = new ADN_TableItem_CheckItem( &tab_, pObj );
        ADN_TableItem_CheckItem* pItemLoadable = new ADN_TableItem_CheckItem( &tab_, pObj );

        // add a new row & set new values
        tab_.setItem( nRow, 0, pItemName );
        tab_.setItem( nRow, 1, pItemNbr );
        tab_.setItem( nRow, 2, pItemMajor );
        tab_.setItem( nRow, 3, pItemLoadable );
        
        // connect items & datas
        pItemName->GetConnector().Connect(&static_cast<ComposanteInfos*>(pObj)->ptrComposante_.GetData()->strName_);
        pItemNbr->GetConnector().Connect(&static_cast<ComposanteInfos*>(pObj)->nNb_);
		pItemMajor->GetConnector().Connect( &static_cast<ComposanteInfos*>(pObj)->bMajor_ );
		pItemLoadable->GetConnector().Connect( &static_cast<ComposanteInfos*>(pObj)->bLoadable_ );
    }
    
private:
    ADN_Units_Composantes_GUI_Connector& operator=( const ADN_Units_Composantes_GUI_Connector& );
};



//-----------------------------------------------------------------------------
// Name: ADN_Units_Composantes_GUI constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Units_Composantes_GUI::ADN_Units_Composantes_GUI(QWidget * parent )
: ADN_Table2             ( parent, "ADN_Units_Composantes_GUI"  )
, bMenuListItemSelected_( false )
{
    // peut etre selectionne & trie
    setSorting(true);
    setSelectionMode(QTable::NoSelection);
    setShowGrid(false);
    setLeftMargin(0);

    // hide vertical header
    verticalHeader()->hide();
    
    // tab with 4 columns
    setNumCols(4);
    setNumRows(0);
    setColumnStretchable(0,true);
    setColumnStretchable(1,true);
    setColumnStretchable(2,true);
    setColumnStretchable(3,true);

    horizontalHeader()->setLabel( 0, tr( "Composantes" ) );
    horizontalHeader()->setLabel( 1, tr( "Nbr" ) );
    horizontalHeader()->setLabel( 2, tr( "Major" ) );
    horizontalHeader()->setLabel( 3, tr( "Loadable" ) );

    // connector creation
    pConnector_=new ADN_Units_Composantes_GUI_Connector(*this);
}


//-----------------------------------------------------------------------------
// Name: ADN_Units_Composantes_GUI destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Units_Composantes_GUI::~ADN_Units_Composantes_GUI()
{
    delete pConnector_;
}


//-----------------------------------------------------------------------------
// Name: ADN_Units_Composantes_GUI::OnContextMenu
// Created: AGN 03-08-04
//-----------------------------------------------------------------------------
void ADN_Units_Composantes_GUI::OnContextMenu(int /*row*/,int /*col*/,const QPoint& pt)
{
    QPopupMenu popupMenu( this );
    QPopupMenu& addMenu = *new QPopupMenu( &popupMenu );

    // Get the list of the possible munitions
    ADN_Composantes_Data::T_ComposanteInfos_Vector& vAllComposantes = ADN_Workspace::GetWorkspace().GetComposantes().GetData().GetComposantes();

    QListView& listView = *new QListView( &addMenu );
    listView.addColumn( "" );
    listView.header()->hide();
    listView.setAllColumnsShowFocus( true );
    listView.setResizeMode( QListView::AllColumns );
    listView.setMinimumSize( 200, std::min( 800, (int)vAllComposantes.size() * 17 ) );

    for( ADN_Composantes_Data::T_ComposanteInfos_Vector::iterator it = vAllComposantes.begin(); it != vAllComposantes.end(); ++it )
    {
        new MT_ValuedListViewItem<int>( (int)(*it), &listView, (*it)->strName_.GetData().c_str() );
    }
    addMenu.insertItem( &listView );

    popupMenu.insertItem( tr( "Add composante"), &addMenu ,0 );
    if ( GetCurrentData() != 0 )
        popupMenu.insertItem( tr( "Remove composante"), 1 );
    
    bMenuListItemSelected_ = false;
    connect( &listView, SIGNAL( doubleClicked( QListViewItem*, const QPoint&, int ) ), this, SLOT( MenuListItemSelected() ) );
    connect( &listView, SIGNAL( returnPressed( QListViewItem* ) ), this, SLOT( MenuListItemSelected() ) );
    int nMenuResult = popupMenu.exec(pt);
    if( nMenuResult == 1 )
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
// Name: ADN_Units_Composantes_GUI::AddNewElement
// Created: AGN 2003-12-08
// -----------------------------------------------------------------------------
void ADN_Units_Composantes_GUI::AddNewElement( int n )
{
    // Get the list of the defined targets
    ComposanteInfos* pNewInfo = new ComposanteInfos();
    pNewInfo->ptrComposante_ = (ADN_Composantes_Data::ComposanteInfos*)(n);

    ADN_Connector_Vector_ABC* pCTable = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
    pCTable->AddItem( pNewInfo );
    pCTable->AddItem( 0 );    
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Composantes_GUI::RemoveCurrentElement
// Created: AGN 2003-12-08
// -----------------------------------------------------------------------------
void ADN_Units_Composantes_GUI::RemoveCurrentElement()
{
    // delete composante
    ComposanteInfos* pCurComposante=(ComposanteInfos*)GetCurrentData();
    if ( pCurComposante )
    {
        // remove current data from list
        // take care cause pCurData_ can change!!
        static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem(pCurComposante);        
    }    
}


// -----------------------------------------------------------------------------
// Name: ADN_Units_Composantes_GUI::MenuListItemSelected
// Created: APE 2005-04-15
// -----------------------------------------------------------------------------
void ADN_Units_Composantes_GUI::MenuListItemSelected()
{
    bMenuListItemSelected_ = true;
    qApp->eventLoop()->exitLoop();
}
