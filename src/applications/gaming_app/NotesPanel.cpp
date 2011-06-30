// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "NotesPanel.h"
#include "moc_NotesPanel.cpp"
#include "clients_gui/ValuedDragObject.h"
#include "clients_kernel/Controller.h"
#include "gaming/Tools.h"
#include "gaming/Note.h"
#include "gaming/Simulation.h"
#include "gaming/NotesModel.h"
#include "protocol/ServerPublisher_ABC.h"
#include "protocol/MessengerSenders.h"

#include <qmessagebox.h>

// -----------------------------------------------------------------------------
// Name: NotesPanel constructor
/** @param  mainWindow
    @param  controllers
    @param  factory
*/
// Created: HBD 2010-01-19
// -----------------------------------------------------------------------------
NotesPanel::NotesPanel( QMainWindow* mainWindow, kernel::Controller& controller, gui::ItemFactory_ABC& factory, NotesModel& model, Publisher_ABC &publisher)
    : QDockWindow( mainWindow, "Notation" )
    , controller_ ( controller )
    , factory_ ( factory )
    , model_ ( model )
    , publisher_( publisher )
{
    setResizeEnabled( true );
    setCloseMode( QDockWindow::Always );
    setCaption( tools::translate( "Notes", "Notations" ) );

    notes_ = new QListView( this );
    notes_->addColumn( tools::translate( "Notes", "Tree" ) );
    notes_->addColumn( tools::translate( "Notes", "Value" ) );
    notes_->addColumn( tools::translate( "Notes", "Text" ) );
    notes_->addColumn( tools::translate( "Notes", "Creation Date" ) );
    notes_->addColumn( tools::translate( "Notes", "Last Update" ) );
    notes_->setColumnWidthMode( 1, QListView::Manual );
    notes_->setColumnWidthMode( 2, QListView::Manual );
    for( int col = 0; col < 5 ; ++col )
        notes_->setColumnAlignment( col, Qt::AlignCenter );

    notes_->setRootIsDecorated( true );
    connect( notes_, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int) ), this, SLOT( OnContextMenu( QListViewItem*, const QPoint&, int) ) );
    setWidget( notes_ );

    noteDialog_ = new NoteDialog( this, publisher_ );
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: NotesPanel destructor
// Created: HBD 2010-01-19
// -----------------------------------------------------------------------------
NotesPanel::~NotesPanel()
{
    controller_.Unregister( *this );
     // $$$$ _RC_ HBD 2010-02-09: Delete notes_ && itemlist
}

// -----------------------------------------------------------------------------
// Name: NotesPanel::FindParentItem
// Created: HBD 2010-02-22
// -----------------------------------------------------------------------------
QListViewItem* NotesPanel::FindItem( unsigned int parent ) const
{
    T_Items::const_iterator it = itemsList_.begin();
    while( it != itemsList_.end() && it->second->GetId() != parent )
        ++it;
    if( it == itemsList_.end() )
        return 0;
    return it->first;
}

// -----------------------------------------------------------------------------
// Name: NotesPanel::FindParentItem
// Created: HBD 2010-02-22
// -----------------------------------------------------------------------------
QListViewItem* NotesPanel::FindItem(const Note* element ) const
{
    T_Items::const_iterator it = itemsList_.begin();
    while( it != itemsList_.end() && it->second != element )
        ++it;
    if( it == itemsList_.end() )
        return 0;
    return it->first;
}

// -----------------------------------------------------------------------------
// Name: NotesPanel::NotifyCreated
/** @param  element
*/
// Created: HBD 2010-01-19
// -----------------------------------------------------------------------------
void NotesPanel::NotifyCreated( const Note& element )
{
    QListViewItem* item = 0;
    unsigned int parent = element.GetParent();
    QListViewItem* parentItem = FindItem( parent );
    if( parent && parentItem )
        item = new QListViewItem( parentItem );
    else
        item = new QListViewItem( notes_ );
    item->setMultiLinesEnabled( true );
    itemsList_[ item ] = &element;
    Display( element, item );
}

// -----------------------------------------------------------------------------
// Name: NotesPanel::NotifyUpdated
/** @param  element
*/
// Created: HBD 2010-01-19
// -----------------------------------------------------------------------------
void NotesPanel::NotifyUpdated( const Note& element )
{
    if( QListViewItem* item = FindItem( &element ))
    {
        if( QListViewItem* oldParent = item->parent() )
        {
           QListViewItem* newParent = FindItem( element.GetParent() );
           if( newParent && newParent != oldParent )
           {
                oldParent->takeItem( item );
                newParent->insertItem( item );
           }
           if( element.GetParent() == 0 )
           {
               oldParent->takeItem( item );
               notes_->insertItem( item );
           }
        }
        Display( element, item );
    }
}

// -----------------------------------------------------------------------------
// Name: NotesPanel::NotifyDeleted
/** @param  element
*/
// Created: HBD 2010-01-19
// -----------------------------------------------------------------------------
void NotesPanel::NotifyDeleted( const Note& element )
{
    if( QListViewItem* item = FindItem( &element ))
    {
        if( element.GetId() == noteDialog_->GetCurrentNoteEdited())
        {
            noteDialog_->SetUpdate( false );
            QMessageBox::information( this, tools::translate( "Notes", "Current note edited has been deleted" ),
                tools::translate( "Notes", "The current note edited will be recreated"), QMessageBox::Yes );
        }
        itemsList_.erase( item );
        delete item;
    }
}

// -----------------------------------------------------------------------------
// Name: ScorePanel::Display
/** @param  score
    @param  item
*/
// Created: HBD 2010-01-19
// -----------------------------------------------------------------------------
void NotesPanel::Display( const Note& note, QListViewItem* item )
{
    item->setDragEnabled( true );
    note.Display( item );
}

// -----------------------------------------------------------------------------
// Name: NotesPanel::OnContextMenu
/** @param  item
    @param  point
*/
// Created: HBD 2010-01-20
// -----------------------------------------------------------------------------

void NotesPanel::OnContextMenu( QListViewItem* item, const QPoint& point, int)
{
    QPopupMenu* menu = new QPopupMenu( notes_ );
    if( item )
    {
        menu->insertItem( tools::translate( "Notes", "Update note" ), this, SLOT( UpdateNote() ) );
        menu->insertSeparator();
        menu->insertItem( tools::translate( "Notes", "Delete note" ), this, SLOT( ConfirmDeleteNote() ) );
        menu->insertItem( tools::translate( "Notes", "Delete note and children notes" ), this,  SLOT( ConfirmDeleteAllTreeNote() ) );
        menu->insertSeparator();
    }
    menu->insertItem( tools::translate( "Notes", "Add note" ), this, SLOT( PreCreationProcess() ) );
    menu->popup( point );
}

// -----------------------------------------------------------------------------
// Name: NotesPanel::ConfirmDeleteAllTreeNote
// Created: HBD 2010-02-05
// -----------------------------------------------------------------------------
void NotesPanel::ConfirmDeleteAllTreeNote()
{
    int result = QMessageBox::question( this,
        tools::translate( "Notes", "Delete note" ),
        tools::translate( "Notes", "Are you sure you want to delete this note and its children?" ),
        tools::translate( "Notes", "Yes" ), tools::translate( "Notes","No" ), QString::null, 0, 1 );

    if( result == 0 )
        if( QListViewItem* item = notes_->selectedItem() )
        {
            T_Items::iterator it = itemsList_.find( item );
            if( it != itemsList_.end())
            {
                plugins::messenger::MarkerDestructionRequest message;
                message().mutable_marker()->set_id( it->second->GetId() );
                message().set_delete_all( true );
                message.Send( publisher_ );
            }
        }
}

// -----------------------------------------------------------------------------
// Name: NotesPanel::ConfirmDeleteNote
// Created: HBD 2010-02-05
// -----------------------------------------------------------------------------
void NotesPanel::ConfirmDeleteNote()
{
    int result = QMessageBox::question( this,
        tools::translate( "Notes", "Delete note" ),
        tools::translate( "Notes", "Are you sure you want to delete this note?" ),
        tools::translate( "Notes", "Yes" ), tools::translate( "Notes", "No" ), QString::null, 0, 1 );

   if( result == 0 )
        if( QListViewItem* item = notes_->selectedItem() )
        {
            T_Items::iterator it = itemsList_.find( item );
            if( it != itemsList_.end())
            {
                plugins::messenger::MarkerDestructionRequest message;
                message().mutable_marker()->set_id( it->second->GetId() );
                message().set_delete_all( false );
                message.Send( publisher_ );
            }
    }
}

// -----------------------------------------------------------------------------
// Name: NotesPanel::PreCreationProcess
// Created: HBD 2010-02-10
// -----------------------------------------------------------------------------
void NotesPanel::PreCreationProcess()
{
    unsigned int parent = 0;
    if( QListViewItem* item = notes_->selectedItem() )
    {
        T_Items::iterator it = itemsList_.find( item );
        if( it != itemsList_.end())
            parent = it->second->GetId();
    }
    noteDialog_->ChangeParent( parent );
    noteDialog_->show();
}

// -----------------------------------------------------------------------------
// Name: NotesPanel::UpdateNote
// Created: HBD 2010-02-10
// -----------------------------------------------------------------------------
void NotesPanel::UpdateNote()
{
   if( QListViewItem* item = notes_->selectedItem() )
   {
       T_Items::iterator it = itemsList_.find( item );
       if( it != itemsList_.end())
           noteDialog_->SetUpdate( *( it->second ) );
   }
    noteDialog_->show();
}

// -----------------------------------------------------------------------------
// Name: NotesPanel::NotifyUpdated
// Created: HBD 2010-03-22
// -----------------------------------------------------------------------------
void NotesPanel::NotifyUpdated( const Simulation& simulation )
{
    if( !simulation.IsConnected() )
    {
        notes_->clear();
        itemsList_.clear();
        model_.Clear();
     }
}
