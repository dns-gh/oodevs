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

#include "adaptation_app_pch.h"
#include "ADN_Units_Composantes_GUI.h"
#include "moc_ADN_Units_Composantes_GUI.cpp"
#include "ADN_App.h"
#include "ADN_Tools.h"
#include "ADN_CommonGfx.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_Units_Data.h"
#include "ADN_Workspace.h"
#include "ADN_TableItem_CheckItem.h"
#include "MT_ValuedListViewItem.h"

typedef ADN_Units_Data::ComposanteInfos ComposanteInfos;

//-----------------------------------------------------------------------------
// Internal Table connector to be connected with
//-----------------------------------------------------------------------------

namespace
{
    class MenuListView : public QWidgetAction
    {
    public:
        explicit MenuListView( ADN_Units_Composantes_GUI* receiver, QObject* parent )
            : QWidgetAction( parent )
            , receiver_( receiver )
            , listView_( 0 )
        {}
        virtual ~MenuListView() {}
        virtual QWidget* createWidget( QWidget* parent )
        {
            ADN_Composantes_Data::T_ComposanteInfos_Vector& vAllComposantes = ADN_Workspace::GetWorkspace().GetComposantes().GetData().GetComposantes();

            listView_ = new Q3ListView( parent );
            listView_->addColumn( "" );
            listView_->header()->hide();
            listView_->setAllColumnsShowFocus( true );
            listView_->setResizeMode( Q3ListView::AllColumns );
            listView_->setMinimumSize( 200, std::min( 500, (int)vAllComposantes.size() * 17 ) );
            for( ADN_Composantes_Data::T_ComposanteInfos_Vector::iterator it = vAllComposantes.begin(); it != vAllComposantes.end(); ++it )
            {
                MT_ValuedListViewItem<int>* pItem = new MT_ValuedListViewItem<int>( (int)(*it), listView_, (*it)->strName_.GetData().c_str() );
                for( int i = 0; i < receiver_->numRows(); ++i )
                    if( !strcmp( QAction::text(), (*it)->strName_.GetData().c_str() ) )
                    {
                        pItem->setEnabled( false );
                        break;
                    }
            }
            connect( listView_, SIGNAL( doubleClicked( Q3ListViewItem*, const QPoint&, int ) ), this, SLOT( MenuListItemSelected() ) );
            connect( listView_, SIGNAL( returnPressed( Q3ListViewItem* ) ), this, SLOT( MenuListItemSelected() ) );            
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
        ADN_Units_Composantes_GUI* receiver_;
        Q3ListView* listView_;
    };
}

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
        ADN_TableItem_String*    pItemName = new ADN_TableItem_String( &tab_, pObj, Q3TableItem::Never );
        ADN_TableItem_Int*       pItemNbr = new ADN_TableItem_Int( &tab_, pObj );
        ADN_TableItem_CheckItem* pItemMajor = new ADN_TableItem_CheckItem( &tab_, pObj );
        ADN_TableItem_CheckItem* pItemLoadable = new ADN_TableItem_CheckItem( &tab_, pObj );
        ADN_TableItem_CheckItem* pItemConveyor = new ADN_TableItem_CheckItem( &tab_, pObj );
        ADN_TableItem_Int*       pItemNbrHuman = new ADN_TableItem_Int( &tab_, pObj );


        // add a new row & set new values
        tab_.setItem( nRow, 0, pItemName );
        tab_.setItem( nRow, 1, pItemNbr );
        tab_.setItem( nRow, 2, pItemMajor );
        tab_.setItem( nRow, 3, pItemLoadable );
        tab_.setItem( nRow, 4, pItemConveyor );
        tab_.setItem( nRow, 5, pItemNbrHuman );

        // connect items & datas
        pItemName->GetConnector().Connect(     &static_cast< ComposanteInfos* >( pObj )->ptrComposante_.GetData()->strName_ );
        pItemNbr->GetConnector().Connect(      &static_cast< ComposanteInfos* >( pObj )->nNb_ );
        pItemMajor->GetConnector().Connect(    &static_cast< ComposanteInfos* >( pObj )->bMajor_ );
        pItemLoadable->GetConnector().Connect( &static_cast< ComposanteInfos* >( pObj )->bLoadable_ );
        pItemConveyor->GetConnector().Connect( &static_cast< ComposanteInfos* >( pObj )->bConveyor_ );
        pItemNbrHuman->GetConnector().Connect( &static_cast< ComposanteInfos* >( pObj )->nNbrHumanInCrew_ );
    }

private:
    ADN_Units_Composantes_GUI_Connector& operator=( const ADN_Units_Composantes_GUI_Connector& );
};

//-----------------------------------------------------------------------------
// Name: ADN_Units_Composantes_GUI constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Units_Composantes_GUI::ADN_Units_Composantes_GUI( QWidget * parent )
    : ADN_Table2            ( parent, "ADN_Units_Composantes_GUI" )
    , bMenuListItemSelected_( false )
{
    // peut etre selectionne & trie
    setSorting( true );
    setSelectionMode( Q3Table::NoSelection );
    setShowGrid( false );
    setLeftMargin( 0 );

    // hide vertical header
    verticalHeader()->hide();

    // tab with 4 columns
    setNumCols( 6 );
    setNumRows( 0 );
    setColumnStretchable( 0, true );

    horizontalHeader()->setLabel( 0, tr( "Equipments" ) );
    horizontalHeader()->setLabel( 1, tr( "Qty" ) );
    horizontalHeader()->setLabel( 2, tr( "Major" ) );
    horizontalHeader()->setLabel( 3, tr( "Loadable" ) );
    horizontalHeader()->setLabel( 4, tr( "Conveyor" ) );
    horizontalHeader()->setLabel( 5, tr( "Crew size" ) );

    // connector creation
    pConnector_ = new ADN_Units_Composantes_GUI_Connector( *this );
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
    Q3PopupMenu popupMenu( this );
    Q3PopupMenu& addMenu = *new Q3PopupMenu( &popupMenu );

    MenuListView* list = new MenuListView( this, &addMenu );
    addMenu.addAction( list );
    // Get the list of the possible munitions

    popupMenu.insertItem( tr( "Add equipment"), &addMenu ,0 );
    if( GetCurrentData() != 0 )
        popupMenu.insertItem( tr( "Remove equipment"), 1 );

    bMenuListItemSelected_ = false;
    int nMenuResult = popupMenu.exec(pt);
    if( nMenuResult == 1 )
        RemoveCurrentElement();
    else if( bMenuListItemSelected_ )
        AddNewElement( list ->SelectedValue() );     
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
    if( pCurComposante )
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
    QApplication::exit_loop();
}
