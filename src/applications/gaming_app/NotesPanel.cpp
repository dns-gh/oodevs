// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "gaming_app_pch.h"
#include "NotesPanel.h"
#include "moc_NotesPanel.cpp"

#include "gaming/Tools.h"
#include "gaming/Note.h"
#include "gaming/NotesModel.h"
#include "clients_kernel/Controller.h"
#include "game_asn/Messenger.h"
#include "game_asn/Publisher_ABC.h"
#include "game_asn/MessengerSenders.h"
#include "clients_gui/ValuedDragObject.h"

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
    setCaption( tools::translate( "NotePanel", "Notations" ) );

    notes_ = new gui::ListDisplayer< NotesPanel > (this, *this, factory_ );
    notes_->AddColumn( tools::translate( "Notes", "Tree" ) );
    notes_->AddColumn( tools::translate( "Notes", "Value" ) );
    notes_->AddColumn( tools::translate( "Notes", "Text" ) );
    notes_->setColumnWidthMode( 2, QListView::Manual );
    notes_->setColumnWidthMode( 3, QListView::Manual );
    notes_->setColumnAlignment( 2, Qt::AlignCenter );
    notes_->setColumnAlignment( 3, Qt::AlignCenter );
    connect( notes_, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int) ), SLOT( OnContextMenu( QListViewItem*, const QPoint&, int) ) );

    setWidget( notes_ );

    noteDialog_ = new NoteDialog( this, publisher_ );
    notes_->setRootIsDecorated( true ); 
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
// Name: NotesPanel::NotifyCreated
/** @param  element 
*/
// Created: HBD 2010-01-19
// -----------------------------------------------------------------------------
void NotesPanel::NotifyCreated( const Note& element )
{
    gui::ValuedListItem* item = 0;
    unsigned int parent = element.GetParent();
    if( parent && itemsList_.find(parent) != itemsList_.end() )
        item = factory_.CreateItem( itemsList_[ parent ] );
    else
        item = factory_.CreateItem( notes_ );
    itemsList_[element.GetId()] = item ;
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
    if( gui::ValuedListItem* item = gui::FindItem( &element, notes_->firstChild() ) )
    {
        QListViewItem* oldParent = item->parent();
        if( oldParent )
        {
           itemIterator it = itemsList_.find( element.GetParent() );
           if (it != itemsList_.end() && (it->second) != oldParent )
           {
                oldParent->takeItem(item);
                it->second->insertItem(item);
           }
           if (element.GetParent() == 0)
           {
               oldParent->takeItem(item);
               notes_->insertItem(item);
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
    if( gui::ValuedListItem* item = gui::FindItem( &element, notes_->firstChild() ) )
        delete item;
}

// -----------------------------------------------------------------------------
// Name: ScorePanel::Display
/** @param  score 
    @param  item 
*/
// Created: HBD 2010-01-19
// -----------------------------------------------------------------------------
void NotesPanel::Display( const Note& note, gui::ValuedListItem* item )
{
    item->setDragEnabled( true );
    item->SetValue( &note );
    note.Display( notes_->GetItemDisplayer( item ) );
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
        menu->insertItem(tools::translate( "Notes", "Update note" ), this, SLOT( UpdateNote()) );
        menu->insertSeparator();
        menu->insertItem(tools::translate( "Notes", "Delete note" ), this, SLOT( ConfirmDeleteNote()) );
        menu->insertItem(tools::translate( "Notes", "Delete note & children notes" ), this,  SLOT( ConfirmDeleteAllTreeNote()));
        menu->insertSeparator();
    }
    menu->insertItem( tools::translate( "Notes", "Add note" ), this, SLOT( PreCreationProcess() ) );
    menu->popup(point);
}


// -----------------------------------------------------------------------------
// Name: NotesPanel::ConfirmDeleteAllTreeNote
// Created: HBD 2010-02-05
// -----------------------------------------------------------------------------
void NotesPanel::ConfirmDeleteAllTreeNote()
{
    int result = QMessageBox::question(this,
        tools::translate("Notes", "Delete note"),
        tools::translate("Notes", "Are you sure you want to delete this note and its children?"),
        tools::translate("Notes","&Yes"), tools::translate("Notes","&No"), QString::null, 0, 1 );

    if (result == 0)
    {
        if ( gui::ValuedListItem* item = static_cast< gui::ValuedListItem* >( notes_->selectedItem()))
        {
            Note* note = item->GetValue< Note >();
            if ( note )
            {
//                for (listIterator it = note->GetChildren(); it !=
                    
                plugins::messenger::NoteDestructionRequest asn;
                asn().id = note->GetId();
                asn().deleteAll = true;
                asn.Send( publisher_ );
            }
        }
    }
}
// -----------------------------------------------------------------------------
// Name: NotesPanel::ConfirmDeleteNote
// Created: HBD 2010-02-05
// -----------------------------------------------------------------------------
void NotesPanel::ConfirmDeleteNote()
{
    int result = QMessageBox::question(this,
        tools::translate("Notes", "Delete note"),
        tools::translate("Notes", "Are you sure you want to delete this note?"),
        tools::translate("Notes","&Yes"), tools::translate("Notes","&No"), QString::null, 0, 1 );

    if (result == 0)
    {
        if ( gui::ValuedListItem* item2 = static_cast< gui::ValuedListItem* >( notes_->selectedItem()))
            if ( Note* note = item2->GetValue< Note >() ) 
            {
                plugins::messenger::NoteDestructionRequest asn;
                asn().id = note->GetId();
                asn().deleteAll = false;
                asn.Send(publisher_);
            }      
    }
}

// -----------------------------------------------------------------------------
// Name: NotesPanel::PreCreationProcess
// Created: HBD 2010-02-10
// -----------------------------------------------------------------------------
void NotesPanel::PreCreationProcess()
{
    noteDialog_->setParent( 0 );

    if (notes_->selectedItem())
    {
        gui::ValuedListItem* item = static_cast< gui::ValuedListItem* >( notes_->selectedItem() );
        if ( Note* note = item->GetValue< Note >() ) 
        {
            noteDialog_->setParent( note->GetId() );
        }
     }
    noteDialog_->show(); 
}

// -----------------------------------------------------------------------------
// Name: NotesPanel::UpdateNote
// Created: HBD 2010-02-10
// -----------------------------------------------------------------------------
void NotesPanel::UpdateNote()
{
    if (notes_->selectedItem())
    {
        gui::ValuedListItem* item = static_cast< gui::ValuedListItem* >( notes_->selectedItem() );
        if ( Note* note = item->GetValue< Note >() ) 
        {
            noteDialog_->setUpdate( *note );
        }
    }
    noteDialog_->show(); 
}
