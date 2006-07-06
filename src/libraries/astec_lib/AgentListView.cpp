// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "AgentListView.h"
#include "Controller.h"
#include "ActionController.h"
#include "ValuedListItem.h"
#include "Controllers.h"

#include "Agent.h"
#include "Team.h"
#include "KnowledgeGroup.h"
#include "AutomatDecisions.h"
#include "OptionVariant.h"
#include "ASN_Messages.h"
#include "ItemFactory_ABC.h"

#include "moc_AgentListView.cpp"

const char* AgentListView::agentMimeType_ = agentMimeType_;

// -----------------------------------------------------------------------------
// Name: AgentListView constructor
// Created: APE 2004-03-18
// -----------------------------------------------------------------------------
AgentListView::AgentListView( QWidget* pParent, Controllers& controllers, Publisher_ABC& publisher, ItemFactory_ABC& factory )
    : ListView< AgentListView >( pParent, *this, factory )
    , controllers_( controllers )
    , publisher_( publisher )
    , factory_( factory )
    , currentTeam_( 0 )
{
    setMinimumSize( 1, 1 );
    addColumn( "Unités" );
    setRootIsDecorated( true );
    setAcceptDrops( true );

    connect( this, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnContextMenuRequested( QListViewItem*, const QPoint&, int ) ) );
    connect( this, SIGNAL( doubleClicked       ( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestCenter() ) );
    connect( this, SIGNAL( spacePressed        ( QListViewItem* ) ),                     this, SLOT( OnRequestCenter() ) );
    connect( this, SIGNAL( selectionChanged    ( QListViewItem* ) ),                     this, SLOT( OnSelectionChange( QListViewItem* ) ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentListView destructor
// Created: APE 2004-03-18
// -----------------------------------------------------------------------------
AgentListView::~AgentListView()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyCreated
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void AgentListView::NotifyCreated( const Team& team )
{
    factory_.CreateItem( this )->Set( &team, team.GetName().c_str() );
    NotifyUpdated( team );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Update
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
template< typename T >
void AgentListView::Update( const T& value )
{
    ValuedListItem* item = FindItem( &value, firstChild() );
    if( ! item )
        throw std::runtime_error( std::string( "Could not update item " ) + typeid( value ).name() + " '" + value.GetName() + "'" );
    Display( value, item );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyUpdated
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void AgentListView::NotifyUpdated( const Team& team )
{
    Update( team );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyDeleted
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void AgentListView::NotifyDeleted( const Team& team )
{
    delete FindSibling( &team, firstChild() );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyUpdated
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentListView::NotifyUpdated( const KnowledgeGroup& group )
{
    Update( group );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyUpdated
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentListView::NotifyUpdated( const Agent& agent )
{
    Update( agent );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::RecursiveDisplay
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
template< typename ParentType, typename ChildType >
void AgentListView::RecursiveDisplay( const ParentType& value, ValuedListItem* item )
{
    DeleteTail( ListView< AgentListView >::Display( value.CreateIterator(), item ) );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Display
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentListView::Display( const Team& team, ValuedListItem* item )
{
    item->Set( &team, team.GetName().c_str() );
    item->setDropEnabled( true );
    item->setDragEnabled( true );
    RecursiveDisplay< Team, KnowledgeGroup >( team, item );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Display
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentListView::Display( const KnowledgeGroup& group, ValuedListItem* item )
{
    item->Set( &group, group.GetName().c_str());
    item->setDropEnabled( true );
    item->setDragEnabled( true );
    RecursiveDisplay< KnowledgeGroup, Agent >( group, item );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Display
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentListView::Display( const Agent& agent, ValuedListItem* item )
{
    item->Set( &agent, agent.GetName().c_str() );
    item->setDropEnabled( true );
    item->setDragEnabled( true );
    if( agent.Retrieve< AutomatDecisions >() )
    {
        const QPixmap pix = agent.Get< AutomatDecisions >().IsEmbraye() ? MAKE_PIXMAP( embraye ) : MAKE_PIXMAP( debraye );
        item->setPixmap( 0, pix );
        RecursiveDisplay< Agent, Agent >( agent, item );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyUpdated
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void AgentListView::NotifyUpdated( const AutomatDecisions& decisions )
{
    ValuedListItem* item = FindItem( & decisions.GetAgent(), firstChild() );
    if( item )
        item->setPixmap( 0, decisions.IsEmbraye() ? MAKE_PIXMAP( embraye ) : MAKE_PIXMAP( debraye ) );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::OnSelectionChange
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentListView::OnSelectionChange( QListViewItem* i )
{
    ValuedListItem* item = (ValuedListItem*)( i );
    item->Select( controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::OnContextMenuRequested
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void AgentListView::OnContextMenuRequested( QListViewItem* i, const QPoint& pos, int )
{
    if( i )
    {
        ValuedListItem* item = (ValuedListItem*)( i );
        item->ContextMenu( controllers_.actions_, pos );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentListView::OnRequestCenter
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
void AgentListView::OnRequestCenter()
{
    if( selectedItem() )
    {
        ValuedListItem* item = (ValuedListItem*)( selectedItem() );
        item->Activate( controllers_.actions_ );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentListView::sizeHint
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
QSize AgentListView::sizeHint() const
{
    return QSize( 230, 340 );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Select
// Created: AGE 2006-03-21
// -----------------------------------------------------------------------------
void AgentListView::Select( const Team& element )
{
    setSelected( FindItem( &element, firstChild() ), true );
    ensureItemVisible( selectedItem() );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Select
// Created: AGE 2006-03-21
// -----------------------------------------------------------------------------
void AgentListView::Select( const KnowledgeGroup& element )
{
    setSelected( FindItem( &element, firstChild() ), true );
    ensureItemVisible( selectedItem() );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::Select
// Created: AGE 2006-03-21
// -----------------------------------------------------------------------------
void AgentListView::Select( const Agent& element )
{
    setSelected( FindItem( &element, firstChild() ), true );
    ensureItemVisible( selectedItem() );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::BeforeSelection
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void AgentListView::BeforeSelection()
{
    selectAll( false );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::AfterSelection
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void AgentListView::AfterSelection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentListView::OptionChanged
// Created: AGE 2006-03-27
// -----------------------------------------------------------------------------
void AgentListView::OptionChanged( const std::string& name, const OptionVariant& value )
{
    if( name == "CurrentTeam" )
        currentTeam_ = value.To< const Team* >();
    ValuedListItem* item = (ValuedListItem*)( firstChild() );
    while( item )
    {
        item->setVisible( ! currentTeam_ || item->Holds( currentTeam_ ) );
        item = (ValuedListItem*)( item->nextSibling() );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentListView::dragObject
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
QDragObject* AgentListView::dragObject()
{
    QListViewItem* pItem = selectedItem();
    if( !pItem )
        return 0;

    QByteArray* pBytes = new QByteArray();
    pBytes->setRawData( (const char*)&pItem, sizeof( QListViewItem* ) );
    QStoredDrag* data = new QStoredDrag( agentMimeType_, this );
    data->setEncodedData( *pBytes );
    return data;
}

// -----------------------------------------------------------------------------
// Name: AgentListView::dropEvent
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
void AgentListView::dropEvent( QDropEvent* pEvent )
{
    if( !pEvent->provides( agentMimeType_ ) )
         return;

    QByteArray tmp = pEvent->encodedData( agentMimeType_ );

    ValuedListItem* pItemToDrop = *reinterpret_cast< ValuedListItem** >( tmp.data() );

    QPoint position = viewport()->mapFromParent( pEvent->pos() );
    ValuedListItem* pItemWhereToDrop = (ValuedListItem*)itemAt( position );
    if( !pItemToDrop || !pItemWhereToDrop || !pItemToDrop->IsA< const Agent* >() )
    {
        pEvent->ignore();
        return;
    }

    if( pItemWhereToDrop->IsA< const Agent* >() )
    {
        const Agent& agent    = *pItemToDrop->GetValue< const Agent* >();
        const Agent& superior = *pItemWhereToDrop->GetValue< const Agent* >();
        if( agent.GetSuperior() == 0 || agent.GetSuperior() == &superior )
        {
            pEvent->ignore();
            return;
        }
        unsigned int superiorId = superior.GetId();
        if( superior.GetSuperior() != 0 )
            superiorId = superior.GetSuperior()->GetId();

        ASN_MsgChangeAutomate asnMsg;
        asnMsg.GetAsnMsg().oid_pion     = agent.GetId();
        asnMsg.GetAsnMsg().oid_automate = superiorId;
        asnMsg.Send( publisher_ );

        pEvent->accept();
    }
    else if( pItemWhereToDrop->IsA< const KnowledgeGroup* >() )
    {
        const Agent&          agent = *pItemToDrop->GetValue< const Agent* >();
        const KnowledgeGroup& kg    = *pItemWhereToDrop->GetValue< const KnowledgeGroup* >();

        if( agent.GetSuperior() != 0 )
        {
            pEvent->ignore();
            return;
        }
        ASN_MsgChangeGroupeConnaissance asnMsg;
        asnMsg.GetAsnMsg().oid_automate            = agent.GetId();
        asnMsg.GetAsnMsg().oid_camp                = kg.GetTeam().GetId();
        asnMsg.GetAsnMsg().oid_groupe_connaissance = kg.GetId();
        asnMsg.Send( publisher_ );

        pEvent->accept();
    }
}

// -----------------------------------------------------------------------------
// Name: AgentListView::dragEnterEvent
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
void AgentListView::dragEnterEvent( QDragEnterEvent* pEvent )
{
    pEvent->accept( pEvent->provides( agentMimeType_ ) );
}

// -----------------------------------------------------------------------------
// Name: AgentListView::NotifyActivated
// Created: AGE 2006-07-04
// -----------------------------------------------------------------------------
void AgentListView::NotifyActivated( const Agent& element )
{
    ValuedListItem* item = FindItem( &element, firstChild() );    
    if( item )
        ensureItemVisible( item );
}
