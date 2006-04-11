// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-08-05 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ObjectListView.cpp $
// $Author: Ape $
// $Modtime: 4/11/04 11:16 $
// $Revision: 6 $
// $Workfile: ObjectListView.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "ObjectListView.h"
#include "moc_ObjectListView.cpp"

#include "App.h"
#include "Object_ABC.h"
#include "ObjectManager.h"
#include "MainWindow.h"
#include "Options.h"
#include "Team.h"
#include "Tools.h"
#include "SelectedElement.h"
#include "ASN_Messages.h"

// -----------------------------------------------------------------------------
// Name: ObjectListView constructor
/** @param  pParent 
*/
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
ObjectListView::ObjectListView( QWidget* pParent )
    : QListView( pParent )
    , pPopupMenu_   ( 0 )
{
    this->setMinimumSize( 1, 1 );
    this->addColumn( "Objets" );
    this->setRootIsDecorated( true );
    this->setResizeMode( QListView::LastColumn );

    connect( &App::GetApp(), SIGNAL( ObjectCreated( Object_ABC& ) ), this, SLOT( AddObject( Object_ABC& ) ) );
    connect( &App::GetApp(), SIGNAL( ObjectDeleted( Object_ABC& ) ), this, SLOT( RemoveObject( Object_ABC& ) ) );
    connect( &App::GetApp(), SIGNAL( ConnexionStatusChanged( bool ) ), this, SLOT( clear() ) );

    connect( this, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestPopup( QListViewItem*, const QPoint&, int ) ) );
    connect( this, SIGNAL( doubleClicked       ( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestCenter() ) );
    connect( this, SIGNAL( spacePressed        ( QListViewItem* ) ),                     this, SLOT( OnRequestCenter() ) );
    connect( this, SIGNAL( selectionChanged    ( QListViewItem* ) ),                     this, SLOT( OnSelectionChange( QListViewItem* ) ) );

    connect( this, SIGNAL( CenterOnPoint( const MT_Vector2D& ) ),                   &MainWindow::GetMainWindow(), SIGNAL( CenterOnPoint( const MT_Vector2D& ) ) );
    connect( this, SIGNAL( ElementSelected( SelectedElement& ) ),               &MainWindow::GetMainWindow(), SIGNAL( ElementSelected( SelectedElement& ) ) );
    connect( this, SIGNAL( NewPopupMenu( QPopupMenu&, const ActionContext& ) ), &MainWindow::GetMainWindow(), SIGNAL( NewPopupMenu( QPopupMenu&, const ActionContext& ) ) );

    connect( &MainWindow::GetMainWindow(), SIGNAL( ElementSelected( SelectedElement& ) ), this,   SLOT( SetSelectedElement( SelectedElement& ) ) );
    connect( &MainWindow::GetMainWindow(), SIGNAL( TeamChanged() ),                           this,   SLOT( OnTeamChanged() ) );

    if( App::GetApp().IsODBEdition() )
    {
        // Refresh the display
        OnTeamChanged();
    }
}
    

// -----------------------------------------------------------------------------
// Name: ObjectListView destructor
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
ObjectListView::~ObjectListView()
{
}


// -----------------------------------------------------------------------------
// Name: ObjectListView::AddObject
/** @param  object 
*/
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
void ObjectListView::AddObject( Object_ABC& object )
{
    int nPlayedTeam = App::GetApp().GetOptions().nPlayedTeam_;
    if( nPlayedTeam != Options::eController && nPlayedTeam != (int)object.GetTeam().GetIdx() )
        return;

    // Check if the agent is not already inserted.
#ifdef _DEBUG
    QListViewItemIterator it( this->firstChild() );
    while( it.current() )
    {
        if( this->ToObject( it.current() ) == &object )
        {
            assert( false );
            return;
        }
        ++it;
    }
#endif // DEBUG

    // Team
    const Team& team = object.GetTeam();
    QListViewItem* pTeamItem = this->firstChild();
    while( pTeamItem != 0 )
    {
        MT_ValuedListViewItem< const Team*, eTeam >* pCastItem = (MT_ValuedListViewItem< const Team*, (int)eTeam >*)pTeamItem;
        if( pCastItem->GetValue() == &team )
            break;
        pTeamItem = pTeamItem->nextSibling();
    }

    if( pTeamItem == 0 )
        pTeamItem = new MT_ValuedListViewItem< const Team*, eTeam >( &team, this, team.GetName().c_str() );

    // Object type
    ASN1T_EnumObjectType nType = object.GetType();
    QListViewItem* pTypeItem = pTeamItem->firstChild();
    while( pTypeItem != 0 )
    {
        MT_ValuedListViewItem< ASN1T_EnumObjectType, eObjectType >* pCastItem = (MT_ValuedListViewItem< ASN1T_EnumObjectType, eObjectType >*)pTypeItem;
        if( pCastItem->GetValue() == nType )
            break;
        pTypeItem = pTypeItem->nextSibling();
    }

    if( pTypeItem == 0 )
        pTypeItem = new MT_ValuedListViewItem< ASN1T_EnumObjectType, eObjectType >( nType, pTeamItem, ENT_Tr::ConvertFromObjectType( (E_ObjectType)nType ).c_str() );

    new MT_ValuedListViewItem< Object_ABC*, eObject >( &object, pTypeItem, QString::number( object.GetID() ) );
}


// -----------------------------------------------------------------------------
// Name: ObjectListView::RemoveObject
/** @param  object 
*/
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
void ObjectListView::RemoveObject( Object_ABC& object )
{
    QListViewItemIterator it( this );
    while( it.current() )
    {
        if( it.current()->rtti() == eObject )
        {
            MT_ValuedListViewItem< Object_ABC*, eObject >* pCastItem = (MT_ValuedListViewItem< Object_ABC*, eObject >*)(it.current());
            if( pCastItem->GetValue() == &object )
            {
                delete it.current();
                return;
            }
        }
        ++it;
    }
}


// -----------------------------------------------------------------------------
// Name: ObjectListView::SetSelectedElement
/** @param  selectedElement 
*/
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
void ObjectListView::SetSelectedElement( SelectedElement& selectedElement )
{
    if( selectedElement.pObject_ == 0 )
    {
        this->selectAll( false );
        return;
    }

    QListViewItemIterator it( this );
    while( it.current() )
    {
        if( it.current()->rtti() == eObject )
        {
            MT_ValuedListViewItem< Object_ABC*, eObject >* pCastItem = (MT_ValuedListViewItem< Object_ABC*, eObject >*)(it.current());
            if( pCastItem->GetValue() == selectedElement.pObject_ )
            {
                disconnect( this, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChange( QListViewItem* ) ) );
                this->setSelected( it.current(), true );
                connect( this, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChange( QListViewItem* ) ) );
                this->ensureItemVisible( it.current() );
                return;
            }
        }
        ++it;
    }

    // Could not find the object to select.
    assert( false );
}


// -----------------------------------------------------------------------------
// Name: ObjectListView::OnRequestPopup
/** @param  pItem 
    @param  pos 
    @param  nCol 
*/
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
void ObjectListView::OnRequestPopup( QListViewItem* , const QPoint& , int )
{
}


// -----------------------------------------------------------------------------
// Name: ObjectListView::OnRequestCenter
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
void ObjectListView::OnRequestCenter()
{
    Object_ABC* pObject = this->ToObject( this->selectedItem() );
    if( pObject != 0 )
        emit CenterOnPoint( pObject->GetCenter() );
}


// -----------------------------------------------------------------------------
// Name: ObjectListView::OnSelectionChange
/** @param  pItem 
*/
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
void ObjectListView::OnSelectionChange( QListViewItem* pItem )
{
    if( pItem == 0 )
        return;

    Object_ABC* pObject = this->ToObject( pItem );
    SelectedElement selectedElement( *pObject );

    // Disconnect and connect to avoid having the signal come back to us.
    disconnect( &MainWindow::GetMainWindow(), SIGNAL( ElementSelected( SelectedElement& ) ), this,   SLOT( SetSelectedElement( SelectedElement& ) ) );
    emit ElementSelected( selectedElement );
    connect( &MainWindow::GetMainWindow(), SIGNAL( ElementSelected( SelectedElement& ) ), this,   SLOT( SetSelectedElement( SelectedElement& ) ) );
}


// -----------------------------------------------------------------------------
// Name: ObjectListView::OnTeamChanged
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
void ObjectListView::OnTeamChanged()
{
    this->clear();
    const ObjectManager::T_ObjectMap& objects = App::GetApp().GetObjectManager().GetObjects();
    for( ObjectManager::CIT_ObjectMap it = objects.begin(); it != objects.end(); ++it )
        this->AddObject( *(it->second) );
}


// -----------------------------------------------------------------------------
// Name: ObjectListView::keyPressEvent
/** @param  pEvent 
*/
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
void ObjectListView::keyPressEvent( QKeyEvent* pEvent )
{
    if( pEvent->key() == Qt::Key_Delete || pEvent->key() == Qt::Key_Backspace )
    {
        if( this->currentItem() != 0 && this->currentItem()->rtti() == eObject )
        {
            Object_ABC* pObject = this->ToObject( this->currentItem() );
            if( App::GetApp().IsODBEdition() )
            {
                App::GetApp().GetObjectManager().UnregisterObject( *pObject );
                App::GetApp().NotifyObjectDeleted( *pObject );
                delete pObject;
            }
            else
            {
                ASN_MsgObjectMagicAction asnMsg;
                asnMsg.GetAsnMsg().oid_objet = pObject->GetID();
                asnMsg.GetAsnMsg().action.t  = T_MsgObjectMagicAction_action_destroy_object;
                asnMsg.Send( 546 );

                std::stringstream strMsg;
                strMsg << "Demande destruction objet " << pObject->GetID();
                MT_LOG_INFO( strMsg.str().c_str(), eSent, 0 );
            }
        }
        pEvent->accept();
        return;
    }

    QListView::keyPressEvent( pEvent );
}
