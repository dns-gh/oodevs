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
#include "clients_kernel/ContextMenu.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Tools.h"
#include "gaming/Note.h"
#include "gaming/Simulation.h"
#include "gaming/NotesModel.h"
#include "protocol/ServerPublisher_ABC.h"

Q_DECLARE_METATYPE( const Note* )

// -----------------------------------------------------------------------------
// Name: NotesPanel constructor
// Created: HBD 2010-01-19
// -----------------------------------------------------------------------------
NotesPanel::NotesPanel( QMainWindow* mainWindow, kernel::Controllers& controllers, NotesModel& model, Publisher_ABC &publisher)
    : gui::RichDockWidget( controllers, mainWindow, "Notation" )
    , model_ ( model )
    , publisher_( publisher )
{
    setObjectName( "notation" );
    setWindowTitle( tools::translate( "Notes", "Notations" ) );

    notes_ = new QTreeView();
    notes_->setRootIsDecorated( true );
    notes_->setModel( &noteModel_ );
    noteModel_.setColumnCount( 5 );
    notes_->setEditTriggers( 0 );
    notes_->setAllColumnsShowFocus( true );
    notes_->setContextMenuPolicy( Qt::CustomContextMenu );
    notes_->header()->setResizeMode( QHeaderView::ResizeToContents );
    notes_->header()->setResizeMode( 2, QHeaderView::Stretch );
    notes_->header()->setStretchLastSection( false );
    QStringList list;
    list.append( tools::translate( "Notes", "Tree" ) );
    list.append( tools::translate( "Notes", "Value" ) );
    list.append( tools::translate( "Notes", "Text" ) );
    list.append( tools::translate( "Notes", "Creation Date" ) );
    list.append( tools::translate( "Notes", "Last Update" ) );
    noteModel_.setHorizontalHeaderLabels( list );

    connect( notes_, SIGNAL( customContextMenuRequested( const QPoint& ) ), SLOT( OnContextMenu( const QPoint& ) ) );
    setWidget( notes_ );

    noteDialog_ = new NoteDialog( this, publisher_ );
    controllers_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: NotesPanel destructor
// Created: HBD 2010-01-19
// -----------------------------------------------------------------------------
NotesPanel::~NotesPanel()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: NotesPanel::FindParentItem
// Created: HBD 2010-02-22
// -----------------------------------------------------------------------------
QStandardItem* NotesPanel::FindItem( const Note* element ) const
{
    return FindItem( element->GetId() );
}

// -----------------------------------------------------------------------------
// Name: NotesPanel::FindParentItem
// Created: HBD 2010-02-22
// -----------------------------------------------------------------------------
QStandardItem* NotesPanel::FindItem( unsigned int id, QStandardItem* parent /*= 0*/ ) const
{
    if( !parent ) parent = noteModel_.invisibleRootItem();

    for( int row = 0; row < parent->rowCount(); ++row )
    {
        if( parent->child( row )->data( Qt::UserRole ).isValid() && parent->child( row )->data( Qt::UserRole ).value< const Note* >()->GetId() == id )
            return parent->child( row );
        else if( parent->child( row )->rowCount() > 0 )
            if( QStandardItem* item = FindItem( id, parent->child( row ) ) )
                return item;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: NotesPanel::NotifyCreated
/** @param  element
*/
// Created: HBD 2010-01-19
// -----------------------------------------------------------------------------
void NotesPanel::NotifyCreated( const Note& element )
{
    QStandardItem* parentItem = FindItem( element.GetParent() );
    if( parentItem )
        AddNoteInfo( element, parentItem );
    else
        AddNoteInfo( element, noteModel_.invisibleRootItem() );
}

// -----------------------------------------------------------------------------
// Name: NotesPanel::NotifyUpdated
/** @param  element
*/
// Created: HBD 2010-01-19
// -----------------------------------------------------------------------------
void NotesPanel::NotifyUpdated( const Note& element )
{
    if( QStandardItem* item = FindItem( &element ) )
    {
        QStandardItem* parent = item->parent()? item->parent() : noteModel_.invisibleRootItem();
        parent->child( item->row(), 0 )->setText( element.GetName() );
        parent->child( item->row(), 1 )->setText( element.GetNumber() );
        parent->child( item->row(), 2 )->setText( element.GetDesc() );
        parent->child( item->row(), 4 )->setText( element.GetStringLastUpdateTime() );
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
    if( QStandardItem* item = FindItem( &element ) )
    {
        if( element.GetId() == noteDialog_->GetCurrentNoteEdited() )
        {
            noteDialog_->SetUpdate( false );
            QMessageBox::information( this, tools::translate( "Notes", "Current note edited has been deleted" ),
                tools::translate( "Notes", "The current note edited will be recreated"), QMessageBox::Yes );
        }
        if( item->parent() )
            item->parent()->removeRow( item->row() );
        else
            noteModel_.removeRow( item->row() );
    }
}

// -----------------------------------------------------------------------------
// Name: ScorePanel::AddNoteInfo
// Created: HBD 2010-01-19
// -----------------------------------------------------------------------------
void NotesPanel::AddNoteInfo( const Note& note, QStandardItem* parent )
{
    QList< QStandardItem* > list;
    QStandardItem* item = new QStandardItem( note.GetName() );
    item->setData( QVariant::fromValue( &note ), Qt::UserRole );
    list.append( item );
    list.append( new QStandardItem( note.GetNumber() ) );
    list.append( new QStandardItem( note.GetDesc() ) );
    list.append( new QStandardItem( note.GetStringCreationTime() ) );
    list.append( new QStandardItem( note.GetStringLastUpdateTime() ) );
    parent->appendRow( list );
}

// -----------------------------------------------------------------------------
// Name: NotesPanel::OnContextMenu
/** @param  item
    @param  point
*/
// Created: HBD 2010-01-20
// -----------------------------------------------------------------------------

void NotesPanel::OnContextMenu( const QPoint& point )
{
    kernel::ContextMenu* menu = new kernel::ContextMenu( notes_ );
    connect( menu, SIGNAL( aboutToHide() ), menu, SLOT( deleteLater() ) );
    const QModelIndex index = notes_->indexAt( notes_->mapTo( notes_, point ) );
    if( index.isValid() )
    {
        if( QStandardItem* item = noteModel_.itemFromIndex( index ) )
        {
            menu->insertItem( tools::translate( "Notes", "Update note" ), this, SLOT( UpdateNote() ) );
            menu->insertSeparator();
            menu->insertItem( tools::translate( "Notes", "Delete note" ), this, SLOT( ConfirmDeleteNote() ) );
            menu->insertItem( tools::translate( "Notes", "Add note" ), this, SLOT( AddSubNote() ) );
        }
    }
    else
        menu->insertItem( tools::translate( "Notes", "Add note" ), this, SLOT( AddRootNote() ) );
    menu->popup( notes_->viewport()->mapToGlobal( point ) );
}

// -----------------------------------------------------------------------------
// Name: NotesPanel::ConfirmDeleteNote
// Created: HBD 2010-02-05
// -----------------------------------------------------------------------------
void NotesPanel::ConfirmDeleteNote()
{
    int result = QMessageBox::question( this,
        tools::translate( "Notes", "Delete note" ),
        tools::translate( "Notes", "Are you sure you want to delete this note and its children?" ),
        tools::translate( "Notes", "Yes" ), tools::translate( "Notes","No" ), QString::null, 0, 1 );

    if( result == 0 )
    {
        const QModelIndex index = notes_->selectionModel()->currentIndex();
        if( !index.isValid() )
            return;
        QStandardItem* item = noteModel_.itemFromIndex( index )->parent()? noteModel_.itemFromIndex( index )->parent()->child( index.row() ) : noteModel_.item( index.row() );
        if( item )
            if( const Note* note = item->data( Qt::UserRole ).value< const Note* >() )
            {
                plugins::messenger::MarkerDestructionRequest message;
                message().mutable_marker()->set_id( note->GetId() );
                message().set_delete_all( true );
                message.Send( publisher_ );
            }
    }
}

// -----------------------------------------------------------------------------
// Name: NotesPanel::AddSubNote
// Created: NPT 2012-10-29
// -----------------------------------------------------------------------------
void NotesPanel::AddSubNote()
{
    unsigned int parent = 0;
    const QModelIndex index = notes_->selectionModel()->currentIndex();
    if( index.isValid() )
    {
        QStandardItem* item = noteModel_.itemFromIndex( index )->parent()? noteModel_.itemFromIndex( index )->parent()->child( index.row() ) : noteModel_.item( index.row() );
        if( item )
            if( const Note* note = item->data( Qt::UserRole ).value< const Note* >() )
                parent = note->GetId();
    }
    noteDialog_->ChangeParent( parent );
    noteDialog_->show();
}

// -----------------------------------------------------------------------------
// Name: NotesPanel::AddRootNote
// Created: NPT 2012-10-29
// -----------------------------------------------------------------------------
void NotesPanel::AddRootNote()
{
    noteDialog_->ChangeParent( 0 );
    noteDialog_->show();
    notes_->selectionModel()->clear();
}

// -----------------------------------------------------------------------------
// Name: NotesPanel::UpdateNote
// Created: HBD 2010-02-10
// -----------------------------------------------------------------------------
void NotesPanel::UpdateNote()
{
    const QModelIndex index = notes_->selectionModel()->currentIndex();
    if( !index.isValid() )
        return;
    QStandardItem* item = noteModel_.itemFromIndex( index )->parent()? noteModel_.itemFromIndex( index )->parent()->child( index.row() ) : noteModel_.item( index.row() );
    if( item && item->data( Qt::UserRole ).isValid() )
           noteDialog_->SetUpdate( *( item->data( Qt::UserRole ).value< const Note* >() ) );
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
        noteModel_.removeRows( 0, noteModel_.rowCount() );
        model_.Clear();
     }
}
