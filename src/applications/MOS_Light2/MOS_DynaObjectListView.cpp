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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_DynaObjectListView.cpp $
// $Author: Ape $
// $Modtime: 4/11/04 11:16 $
// $Revision: 6 $
// $Workfile: MOS_DynaObjectListView.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_DynaObjectListView.h"
#include "moc_MOS_DynaObjectListView.cpp"

#include "MOS_App.h"
#include "MOS_DynaObject.h"
#include "MOS_DynaObjectManager.h"
#include "MOS_MainWindow.h"
#include "MOS_Options.h"
#include "MOS_Team.h"
#include "MOS_Tools.h"
#include "MOS_SelectedElement.h"
#include "MOS_ASN_Messages.h"

#ifndef MOS_USE_INLINE
#   include "MOS_DynaObjectListView.inl"
#endif


// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectListView constructor
/** @param  pParent 
*/
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
MOS_DynaObjectListView::MOS_DynaObjectListView( QWidget* pParent )
    : QListView( pParent )
    , pPopupMenu_   ( 0 )
{
    this->setMinimumSize( 1, 1 );
    this->addColumn( "Objets" );
    this->setRootIsDecorated( true );
    this->setResizeMode( QListView::LastColumn );

    connect( &MOS_App::GetApp(), SIGNAL( DynaObjectCreated( MOS_DynaObject& ) ), this, SLOT( AddObject( MOS_DynaObject& ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( DynaObjectDeleted( MOS_DynaObject& ) ), this, SLOT( RemoveObject( MOS_DynaObject& ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( ConnexionStatusChanged( bool ) ), this, SLOT( clear() ) );

    connect( this, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestPopup( QListViewItem*, const QPoint&, int ) ) );
    connect( this, SIGNAL( doubleClicked       ( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestCenter() ) );
    connect( this, SIGNAL( spacePressed        ( QListViewItem* ) ),                     this, SLOT( OnRequestCenter() ) );
    connect( this, SIGNAL( selectionChanged    ( QListViewItem* ) ),                     this, SLOT( OnSelectionChange( QListViewItem* ) ) );

    connect( this, SIGNAL( CenterOnPoint( const MT_Vector2D& ) ),                   &MOS_MainWindow::GetMainWindow(), SIGNAL( CenterOnPoint( const MT_Vector2D& ) ) );
    connect( this, SIGNAL( ElementSelected( MOS_SelectedElement& ) ),               &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementSelected( MOS_SelectedElement& ) ) );
    connect( this, SIGNAL( NewPopupMenu( QPopupMenu&, const MOS_ActionContext& ) ), &MOS_MainWindow::GetMainWindow(), SIGNAL( NewPopupMenu( QPopupMenu&, const MOS_ActionContext& ) ) );

    connect( &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementSelected( MOS_SelectedElement& ) ), this,   SLOT( SetSelectedElement( MOS_SelectedElement& ) ) );
    connect( &MOS_MainWindow::GetMainWindow(), SIGNAL( TeamChanged() ),                           this,   SLOT( OnTeamChanged() ) );

    if( MOS_App::GetApp().IsODBEdition() )
    {
        // Refresh the display
        OnTeamChanged();
    }
}
    

// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectListView destructor
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
MOS_DynaObjectListView::~MOS_DynaObjectListView()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectListView::AddObject
/** @param  object 
*/
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
void MOS_DynaObjectListView::AddObject( MOS_DynaObject& object )
{
    int nPlayedTeam = MOS_MainWindow::GetMainWindow().GetOptions().nPlayedTeam_;
    if( nPlayedTeam != MOS_Options::eController && nPlayedTeam != (int)object.GetTeam().GetIdx() )
        return;

    // Check if the agent is not already inserted.
#ifdef MOS_DEBUG
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
#endif // MOS_DEBUG

    // Team
    const MOS_Team& team = object.GetTeam();
    QListViewItem* pTeamItem = this->firstChild();
    while( pTeamItem != 0 )
    {
        MT_ValuedListViewItem< const MOS_Team*, eTeam >* pCastItem = (MT_ValuedListViewItem< const MOS_Team*, (int)eTeam >*)pTeamItem;
        if( pCastItem->GetValue() == &team )
            break;
        pTeamItem = pTeamItem->nextSibling();
    }

    if( pTeamItem == 0 )
        pTeamItem = new MT_ValuedListViewItem< const MOS_Team*, eTeam >( &team, this, team.GetName().c_str() );

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

    new MT_ValuedListViewItem< MOS_DynaObject*, eObject >( &object, pTypeItem, QString::number( object.GetID() ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectListView::RemoveObject
/** @param  object 
*/
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
void MOS_DynaObjectListView::RemoveObject( MOS_DynaObject& object )
{
    QListViewItemIterator it( this );
    while( it.current() )
    {
        if( it.current()->rtti() == eObject )
        {
            MT_ValuedListViewItem< MOS_DynaObject*, eObject >* pCastItem = (MT_ValuedListViewItem< MOS_DynaObject*, eObject >*)(it.current());
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
// Name: MOS_DynaObjectListView::SetSelectedElement
/** @param  selectedElement 
*/
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
void MOS_DynaObjectListView::SetSelectedElement( MOS_SelectedElement& selectedElement )
{
    if( selectedElement.pDynaObject_ == 0 )
    {
        this->selectAll( false );
        return;
    }

    QListViewItemIterator it( this );
    while( it.current() )
    {
        if( it.current()->rtti() == eObject )
        {
            MT_ValuedListViewItem< MOS_DynaObject*, eObject >* pCastItem = (MT_ValuedListViewItem< MOS_DynaObject*, eObject >*)(it.current());
            if( pCastItem->GetValue() == selectedElement.pDynaObject_ )
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
// Name: MOS_DynaObjectListView::OnRequestPopup
/** @param  pItem 
    @param  pos 
    @param  nCol 
*/
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
void MOS_DynaObjectListView::OnRequestPopup( QListViewItem* pItem, const QPoint& pos, int nCol )
{
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectListView::OnRequestCenter
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
void MOS_DynaObjectListView::OnRequestCenter()
{
    MOS_DynaObject* pObject = this->ToObject( this->selectedItem() );
    if( pObject != 0 )
        emit CenterOnPoint( pObject->GetCenter() );
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectListView::OnSelectionChange
/** @param  pItem 
*/
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
void MOS_DynaObjectListView::OnSelectionChange( QListViewItem* pItem )
{
    if( pItem == 0 )
        return;

    MOS_DynaObject* pObject = this->ToObject( pItem );
    MOS_SelectedElement selectedElement( *pObject );

    // Disconnect and connect to avoid having the signal come back to us.
    disconnect( &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementSelected( MOS_SelectedElement& ) ), this,   SLOT( SetSelectedElement( MOS_SelectedElement& ) ) );
    emit ElementSelected( selectedElement );
    connect( &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementSelected( MOS_SelectedElement& ) ), this,   SLOT( SetSelectedElement( MOS_SelectedElement& ) ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectListView::OnTeamChanged
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
void MOS_DynaObjectListView::OnTeamChanged()
{
    this->clear();
    const MOS_DynaObjectManager::T_DynaObjectMap& objects = MOS_App::GetApp().GetDynaObjectManager().GetDynaObjects();
    for( MOS_DynaObjectManager::CIT_DynaObjectMap it = objects.begin(); it != objects.end(); ++it )
        this->AddObject( *(it->second) );
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectListView::keyPressEvent
/** @param  pEvent 
*/
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
void MOS_DynaObjectListView::keyPressEvent( QKeyEvent* pEvent )
{
    if( pEvent->key() == Qt::Key_Delete || pEvent->key() == Qt::Key_Backspace )
    {
        if( this->currentItem() != 0 && this->currentItem()->rtti() == eObject )
        {
            MOS_DynaObject* pObject = this->ToObject( this->currentItem() );
            if( MOS_App::GetApp().IsODBEdition() )
            {
                MOS_App::GetApp().GetDynaObjectManager().UnregisterDynaObject( *pObject );
                MOS_App::GetApp().NotifyDynaObjectDeleted( *pObject );
                delete pObject;
            }
            else
            {
                MOS_ASN_MsgObjectMagicAction asnMsg;
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
