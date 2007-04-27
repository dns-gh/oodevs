// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ListParameter.h"
#include "moc_ListParameter.cpp"
#include "clients_gui/ValuedListItem.h"
#include "ParamVisitor_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ListParameter constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ListParameter::ListParameter( QObject* parent, const QString& name, kernel::ActionController& controller, bool optional )
    : QObject    ( parent )
    , Param_ABC  ( name )
    , controller_( controller )
    , list_      ( 0 )
    , selected_  ( 0 )
    , optional_  ( optional )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ListParameter destructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ListParameter::~ListParameter()
{
    Clear();
    delete list_;
}

// -----------------------------------------------------------------------------
// Name: ListParameter::CheckValidity
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
bool ListParameter::CheckValidity()
{
    if( !list_ || ( !optional_ && !list_->childCount() ) )
        return false;
    if( selected_ )
        if( Param_ABC* param = static_cast< const ValuedListItem* >( selected_ )->GetValue< Param_ABC >() )
            return param->CheckValidity();
    return true;
}

// -----------------------------------------------------------------------------
// Name: ListParameter::BuildInterface
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
void ListParameter::BuildInterface( QWidget* parent )
{
    QVBox* box = new QVBox( parent );
    list_ = new QListView( box );
    list_->addColumn( GetName() );
    list_->setResizeMode( QListView::LastColumn );
    connect( list_, SIGNAL( selectionChanged( QListViewItem* ) ), SLOT( OnSelectionChanged( QListViewItem* ) ) );
    connect( list_, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), SLOT( OnRequestPopup( QListViewItem*, const QPoint& ) ) );
}

// -----------------------------------------------------------------------------
// Name: ListParameter::OnRequestPopup
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
void ListParameter::OnRequestPopup( QListViewItem* item, const QPoint& pos )
{
    QPopupMenu* menu = new QPopupMenu( list_ );
    menu->insertItem( tr( "Create" ), this, SLOT( OnCreate() ) );
    if( item )
        menu->insertItem( tr( "Remove" ), this, SLOT( OnDeleteSelectedItem() ) );
    menu->insertItem( tr( "Clear list" ), this, SLOT( OnClear() ) );
    menu->popup( pos );
}

// -----------------------------------------------------------------------------
// Name: ListParameter::OnCreate
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
void ListParameter::OnCreate()
{
    if( ! list_ || ( list_->childCount() && ! CheckValidity() ) )
        return;
    Param_ABC* param = CreateElement();
    QVBox* widget = new QVBox( list_->parentWidget() );
    widgets_[param] = widget;
    param->BuildInterface( widget );
    ValuedListItem* item = new ValuedListItem( list_, list_->lastItem() );
    item->SetValue( param );
    item->setText( 0, param->GetName() );
    list_->setSelected( item, true );
}

// -----------------------------------------------------------------------------
// Name: ListParameter::OnDeleteSelectedItem
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
void ListParameter::OnDeleteSelectedItem()
{
    if( list_ )
        DeleteItem( list_->selectedItem() );
}

// -----------------------------------------------------------------------------
// Name: ListParameter::OnClear
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
void ListParameter::OnClear()
{
    Clear();
}

// -----------------------------------------------------------------------------
// Name: ListParameter::OnSelectionChanged
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
void ListParameter::OnSelectionChanged( QListViewItem* item )
{
    if( selected_ == item )
        return;
    if( selected_ )
    {
        if( CheckValidity() )
        {
            if( Param_ABC* param = static_cast< ValuedListItem* >( selected_ )->GetValue< Param_ABC >() )
            {
                param->RemoveFromController();
                widgets_[param]->hide();
            }
        }
        else
        {
            list_->setSelected( selected_, true );
            return;
        }
    }
    if( item )
        if( Param_ABC* param = static_cast< ValuedListItem* >( item )->GetValue< Param_ABC >() )
        {
            param->RegisterIn( controller_ );
            widgets_[param]->show();
        }
    selected_ = item;
}

// -----------------------------------------------------------------------------
// Name: ListParameter::DeleteItem
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
void ListParameter::DeleteItem( QListViewItem* item )
{
    if( item == selected_ )
    {
        if( Param_ABC* param = static_cast< ValuedListItem* >( selected_ )->GetValue< Param_ABC >() )
        {
            param->RemoveFromController();
            widgets_[param]->hide();
        }
        selected_->setSelected( false );
        selected_ = 0;
    }
    if( Param_ABC* param = static_cast< ValuedListItem* >( item )->GetValue< Param_ABC >() )
    {
        widgets_.erase( param );
        delete param;
    }
    delete item;
}

// -----------------------------------------------------------------------------
// Name: ListParameter::Clear
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
void ListParameter::Clear()
{
    if( list_ )
        while( list_->childCount() )
            DeleteItem( list_->firstChild() );
}

// -----------------------------------------------------------------------------
// Name: ListParameter::Draw
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
void ListParameter::Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( !list_ )
        return;
    QListViewItemIterator it( list_ );
    for( unsigned int i = 0; it.current(); ++it, ++i )
    {
        ValuedListItem* item = static_cast< ValuedListItem* >( it.current() );
        item->GetValue< Param_ABC >()->Draw( point, viewport, tools );
    }
}

// -----------------------------------------------------------------------------
// Name: ListParameter::Count
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
unsigned int ListParameter::Count() const
{
    return list_->childCount();
}

// -----------------------------------------------------------------------------
// Name: ListParameter::Accept
// Created: SBO 2007-04-27
// -----------------------------------------------------------------------------
void ListParameter::Accept( ParamVisitor_ABC& visitor ) const
{
    if( !list_ )
        return;
    QListViewItemIterator it( list_ );
    for( unsigned int i = 0; it.current(); ++it, ++i )
    {
        ValuedListItem* item = static_cast< ValuedListItem* >( it.current() );
        visitor.Visit( *item->GetValue< Param_ABC >() );
    }
}
