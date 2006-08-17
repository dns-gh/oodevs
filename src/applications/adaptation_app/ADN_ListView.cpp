//*****************************************************************************
//
// $Created: JDY 03-06-27 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ListView.cpp $
// $Author: Ape $
// $Modtime: 25/04/05 10:43 $
// $Revision: 10 $
// $Workfile: ADN_ListView.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_ListView.h"
#include "moc_ADN_ListView.cpp"

#include <qpopupmenu.h>
#include <qtooltip.h>
#include <qpainter.h>
#include <qprinter.h>
#include <qpaintdevicemetrics.h>

#include "ADN_ListViewItem.h"
#include "ADN_Workspace.h"
#include "ADN_Enums.h"
#include "ADN_Connector_ListView_ABC.h"
#include "ADN_App.h"
#include "ADN_MainWindow.h"
#include "ADN_Tools.h"
#include "ADN_GuiTools.h"
#include "ADN_Wizard_ABC.h"


//-----------------------------------------------------------------------------
// Name: ADN_ListView constructor
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_ListView::ADN_ListView( QWidget* pParent, const char* szName, WFlags f )
: QListView         ( pParent, szName, f )
, ADN_Gfx_ABC       ()
, pCurData_         ( 0 )
, pObjectCreator_   ( 0 )
, bDeletionEnabled_ ( false )
, bPrinting_        ( false )
{
    connect( this, SIGNAL( onItem( QListViewItem* ) ), this, SLOT( OnOnItem( QListViewItem* ) ) );

    connect( this, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( SetCurrentItem( QListViewItem* ) ) );
    connect( this, SIGNAL( currentChanged( QListViewItem* ) ), this, SLOT( SetCurrentItem( QListViewItem * ) ) );
    connect( this, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnContextMenuRequested( QListViewItem*, const QPoint &, int ) ) );

    connect( static_cast< ADN_App* >( qApp )->GetMainWindow(), SIGNAL(OpenModeToggled()), this, SLOT(UpdateEnableState()) );
}


//-----------------------------------------------------------------------------
// Name: ADN_ListView destructor
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_ListView::~ADN_ListView()
{
}


//-----------------------------------------------------------------------------
// Name: ADN_ListView::FindNdx
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
int ADN_ListView::FindNdx(void *data)
{
    int ndx=0;
    QListViewItemIterator it( this );
    while ( it.current() != 0 ) 
    {
        ADN_ListViewItem* pCurr=(ADN_ListViewItem*)it.current();
        if ( pCurr->GetData()==data )
            return ndx;
        ++it;
        ++ndx;
    }
    return 0;
}


//-----------------------------------------------------------------------------
// Name: ADN_ListView::ItemAt
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
ADN_ListViewItem* ADN_ListView::ItemAt(int i)
{
    int ndx=0;
    QListViewItemIterator it( this );
    while ( it.current() != 0 && ndx < i ) 
    {
        ++it;
        ++ndx;
    }
    return (ADN_ListViewItem*)it.current();
}


//-----------------------------------------------------------------------------
// Name: ADN_ListView::FindItem
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_ListViewItem* ADN_ListView::FindItem(void* data)
{
    QListViewItemIterator it( this );
    while ( it.current() != 0 ) 
    {
        ADN_ListViewItem* pCurr=(ADN_ListViewItem*)it.current();
        if ( pCurr->GetData()==data )
            return pCurr;
        ++it;
    }
    return 0;
}


inline void SetAutoClear(T_ConnectorVector& v,bool b)
{
    for ( T_ConnectorVector::iterator itConnector=v.begin();itConnector!=v.end();++itConnector)
        (*itConnector)->SetAutoClear(b);
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView::SetCurrentItem
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
void  ADN_ListView::SetCurrentItem( void* pData )
{
    if( pCurData_ == pData )
        return;

    if( pData == 0 )
        SetAutoClear( vItemConnectors_, true );
    
    ConnectItem( false );

    pCurData_ = pData;

    ConnectItem( true );

    if( pData == 0 )
        SetAutoClear( vItemConnectors_, false );

    this->blockSignals( true );
    QListViewItem* pItem = this->FindItem( pData );
    this->setSelected( pItem, true );
    this->blockSignals( false );

    emit ItemSelected( pData );
}


//-----------------------------------------------------------------------------
// Name: ADN_ListView::OnContextMenu
// Created: JDY 03-07-28
//-----------------------------------------------------------------------------
void ADN_ListView::OnContextMenu( const QPoint& /*pt*/ )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_ListView::FillContextMenuWithDefault
// Created: APE 2005-01-28
// -----------------------------------------------------------------------------
void ADN_ListView::FillContextMenuWithDefault( QPopupMenu& popupMenu, ADN_ObjectCreator_ABC& objectCreator )
{
    pObjectCreator_ = &objectCreator;
    popupMenu.insertItem( tr( "Create new" ), this, SLOT( ContextMenuNew() ) );
    if( pCurData_ != 0 )
        popupMenu.insertItem( tr( "Delete" ), this, SLOT( ContextMenuDelete() ) );
}


// -----------------------------------------------------------------------------
// Name: ADN_ListView::ContextMenuNew
// Created: APE 2005-01-28
// -----------------------------------------------------------------------------
void ADN_ListView::ContextMenuNew()
{
    // Make sure we were given an object creator.
    assert( pObjectCreator_ != 0 );

    ADN_Ref_ABC* pNewObject = pObjectCreator_->CreateObject();
    if( pNewObject == 0 )
    {
        // We've used the object creator (even if it did not manage to create an object),
        // we'll need to be given another one for next time.
        pObjectCreator_ = 0;
        return;
    }

    ADN_Connector_Vector_ABC* pCList = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
    pCList->AddItem( pNewObject );

    // Set the new item as current item.
    setCurrentItem( FindItem( pNewObject ) );

    // We've used the object creator, we'll need to be given another one for next time.
    pObjectCreator_ = 0;
}


// -----------------------------------------------------------------------------
// Name: ADN_ListView::ContextMenuDelete
// Created: APE 2005-01-27
// -----------------------------------------------------------------------------
void ADN_ListView::ContextMenuDelete()
{
    if( pCurData_ == 0 )
        return;

    ADN_Ref_ABC* pCurrentData = (ADN_Ref_ABC*)pCurData_;

    // Check if the item is multi-referenced, and warn the user if it's the case.
    if( pCurrentData->IsMultiRef() )
        if( ! ADN_GuiTools::MultiRefWarning() )
            return;

    // Remove the item from the list.
    static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( pCurrentData );
    //$$$$ delete it?
}


//-----------------------------------------------------------------------------
// Name: ADN_ListView::SetCurrentItem
// Created: JDY 03-07-02
//-----------------------------------------------------------------------------
void  ADN_ListView::SetCurrentItem( QListViewItem* pItem )
{
    if( pItem != 0 )
        SetCurrentItem( static_cast<ADN_ListViewItem*>( pItem )->GetData() );   
    else
        SetCurrentItem( (void*)0 );
}


//-----------------------------------------------------------------------------
// Name: ADN_ListView::OnContextMenuRequested
// Created: JDY 03-07-28
//-----------------------------------------------------------------------------
void ADN_ListView::OnContextMenuRequested( QListViewItem* /*pItem*/, const QPoint& pt, int /*nCol*/ )
{
    OnContextMenu( pt );  
}


// -----------------------------------------------------------------------------
// Name: ADN_ListView::setEnabled
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
void ADN_ListView::setEnabled( bool b )
{
    if( bEnabledOnlyInAdminMode_ && b )
        QListView::setEnabled( ADN_Workspace::GetWorkspace().GetOpenMode() == eOpenMode_Admin );
    else
        QListView::setEnabled( b );
}


// -----------------------------------------------------------------------------
// Name: ADN_ListView::UpdateEnableState
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
void ADN_ListView::UpdateEnableState()
{
    if( bEnabledOnlyInAdminMode_ && IsAutoEnabled() )
        setEnabled( static_cast< ADN_Connector_ListView_ABC* >( pConnector_ )->IsConnected() );
}


// -----------------------------------------------------------------------------
// Name: ADN_ListView::keyReleaseEvent
// Created: APE 2005-04-06
// -----------------------------------------------------------------------------
void ADN_ListView::keyReleaseEvent( QKeyEvent* pEvent )
{
    if( pCurData_ == 0 || ! bDeletionEnabled_ )
        return;

    if( pEvent->key() == Qt::Key_BackSpace || pEvent->key() == Qt::Key_Delete )
    {
        ADN_Ref_ABC* pCurrentData = (ADN_Ref_ABC*)pCurData_;

        if( ! ADN_GuiTools::DeletionWarning() )
            return;

        // Remove the item from the list.
        static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( pCurrentData );   
    }
}


// -----------------------------------------------------------------------------
// Name: ADN_ListView::OnOnItem
// Created: APE 2005-04-25
// -----------------------------------------------------------------------------
void ADN_ListView::OnOnItem( QListViewItem* pItem )
{
    if( pItem == 0 )
        return;

    std::string strToolTip = this->GetToolTipFor( *pItem );
    if( strToolTip == "" )
        return;

    QToolTip::remove( this, toolTipRect_ );
    QRect itemRect = this->itemRect( pItem );
    QToolTip::add( this->viewport() , itemRect, strToolTip.c_str() );
    toolTipRect_ = itemRect;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView::ComputeNbrPrintPages
// Created: APE 2005-04-04
// -----------------------------------------------------------------------------
int ADN_ListView::ComputeNbrPrintPages( const QSize& painterSize ) const
{
    int nWidthInPages = ceil( (float)this->contentsWidth() / painterSize.width() );
    int nHeightInPages = ceil( (float)this->contentsHeight() / painterSize.height() );
    return nWidthInPages * nHeightInPages;
}


// -----------------------------------------------------------------------------
// Name: ADN_ListView::Print
// Created: APE 2005-04-04
// -----------------------------------------------------------------------------
void ADN_ListView::Print( int nPage, QPainter& painter, const QSize& painterSize )
{
    bPrinting_ = true;
    // Ready the table.
    clearSelection();

    int nWidthInPages = ceil( (float)this->contentsWidth() / painterSize.width() );

    int nY = floor( (float)nPage / nWidthInPages );
    int nX = nPage % nWidthInPages;

    painter.save();
    painter.translate( -nX * painterSize.width(), -nY * painterSize.height() );
    QListView::drawContentsOffset( &painter, 0, 0, nX * painterSize.width(), nY * painterSize.height(), painterSize.width(), painterSize.height() );
    painter.restore();
    bPrinting_ = false;
}