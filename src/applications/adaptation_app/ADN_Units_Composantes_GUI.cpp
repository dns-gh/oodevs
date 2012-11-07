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
#include "ADN_MenuListView.h"
#include "ADN_Units_Data.h"
#include "ADN_Workspace.h"
#include "ADN_TableItem_CheckItem.h"
#include "MT_ValuedListViewItem.h"

typedef ADN_Composantes_Data::ComposanteInfos   ComposanteInfos;
typedef ADN_Units_Data::ComposanteInfos         UnitComposanteInfos;

Q_DECLARE_METATYPE( ComposanteInfos* )

//-----------------------------------------------------------------------------
// Name: ADN_Units_Composantes_GUI constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Units_Composantes_GUI::ADN_Units_Composantes_GUI( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /* = 0 */ )
    : ADN_Table3            ( objectName, connector, pParent )
    , bMenuListItemSelected_( false )
{
    dataModel_.setColumnCount( 6 );
    QStringList horizontalHeaders;
    horizontalHeaders << tr( "Equipments" )
                      << tr( "Qty" )
                      << tr( "Major" )
                      << tr( "Loadable" )
                      << tr( "Conveyor" )
                      << tr( "Crew size" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( 0, QHeaderView::Stretch );
    horizontalHeader()->setResizeMode( 1, QHeaderView::ResizeToContents );
    horizontalHeader()->setResizeMode( 2, QHeaderView::ResizeToContents );
    horizontalHeader()->setResizeMode( 3, QHeaderView::ResizeToContents );
    horizontalHeader()->setResizeMode( 4, QHeaderView::ResizeToContents );
    horizontalHeader()->setResizeMode( 5, QHeaderView::ResizeToContents );
    verticalHeader()->setVisible( false );

    delegate_.AddSpinBoxOnColumn( 1, 0, std::numeric_limits< int >::max() );
    delegate_.AddCheckBoxOnColumn( 2 );
    delegate_.AddCheckBoxOnColumn( 3 );
    delegate_.AddCheckBoxOnColumn( 4 );
    delegate_.AddSpinBoxOnColumn( 5, 0, std::numeric_limits< int >::max() );
}

//-----------------------------------------------------------------------------
// Name: ADN_Units_Composantes_GUI destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Units_Composantes_GUI::~ADN_Units_Composantes_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Composantes_GUI::AddRow
// Created: ABR 2012-11-06
// -----------------------------------------------------------------------------
void ADN_Units_Composantes_GUI::AddRow( int row, void* data )
{
    UnitComposanteInfos* composante = static_cast< UnitComposanteInfos* >( data );
    if( !composante )
        return;

    AddItem( row, 0, data, &composante->ptrComposante_.GetData()->strName_, ADN_StandardItem::eString );
    AddItem( row, 1, data, &composante->nNb_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
    AddItem( row, 2, data, &composante->bMajor_, ADN_StandardItem::eBool, Qt::ItemIsEditable );
    AddItem( row, 3, data, &composante->bLoadable_, ADN_StandardItem::eBool, Qt::ItemIsEditable );
    AddItem( row, 4, data, &composante->bConveyor_, ADN_StandardItem::eBool, Qt::ItemIsEditable );
    AddItem( row, 5, data, &composante->nNbrHumanInCrew_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
}

//-----------------------------------------------------------------------------
// Name: ADN_Units_Composantes_GUI::OnContextMenu
// Created: AGN 03-08-04
//-----------------------------------------------------------------------------
void ADN_Units_Composantes_GUI::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    Q3PopupMenu& addMenu = *new Q3PopupMenu( &popupMenu );

    ADN_MenuListView< ComposanteInfos >* list = new ADN_MenuListView< ComposanteInfos >( this, ADN_Workspace::GetWorkspace().GetComposantes().GetData().GetComposantes(), &addMenu );
    addMenu.addAction( list );
    // Get the list of the possible munitions

    popupMenu.insertItem( tr( "Add equipment"), &addMenu ,0 );
    if( GetSelectedData() != 0 )
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
void ADN_Units_Composantes_GUI::AddNewElement( ComposanteInfos* info )
{
    if ( !info )
        return;

    UnitComposanteInfos* pNewInfo = new UnitComposanteInfos();
    pNewInfo->ptrComposante_ = info;
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
    UnitComposanteInfos* param = static_cast< UnitComposanteInfos* >( GetSelectedData() );
    if( param )
        static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( param );
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
