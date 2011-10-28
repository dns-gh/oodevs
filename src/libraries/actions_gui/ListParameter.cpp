// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ListParameter.h"
#include "moc_ListParameter.cpp"
#include "clients_gui/ValuedListItem.h"
#include "clients_gui/Tools.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ListParameter constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ListParameter::ListParameter( QObject* parent, const kernel::OrderParameter& parameter, kernel::ActionController& controller )
    : QObject       ( parent )
    , Param_ABC     ( parameter.GetName().c_str() )
    , controller_   ( controller )
    , list_         ( 0 )
    , selected_     ( 0 )
    , min_          ( parameter.MinOccurs() )
    , max_          ( parameter.MaxOccurs() )
    , optional_     ( parameter.IsOptional() )
    , createEnabled_( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ListParameter destructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ListParameter::~ListParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ListParameter::CheckValidity
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
bool ListParameter::CheckValidity()
{
    if( !list_ )
        return Invalid();
    if( !optional_ )
    {
        unsigned int children = list_->childCount();
        if( min_ > children || max_ < children )
            return Invalid();
    }
    if( selected_ )
        if( Param_ABC* param = static_cast< const ::gui::ValuedListItem* >( selected_ )->GetValue< Param_ABC >() )
            return param->CheckValidity() ? true : Invalid();
    return true;
}

// -----------------------------------------------------------------------------
// Name: ListParameter::BuildInterface
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
QWidget* ListParameter::BuildInterface( QWidget* parent )
{
    Q3VBox* box = new Q3VBox( parent );
    list_ = new Q3ListView( box );
    list_->setSorting( -1 );
    list_->setMaximumHeight( 100 );
    list_->addColumn( GetName() );
    list_->setResizeMode( Q3ListView::LastColumn );
    connect( list_, SIGNAL( selectionChanged( Q3ListViewItem* ) ), SLOT( OnSelectionChanged( Q3ListViewItem* ) ) );
    connect( list_, SIGNAL( contextMenuRequested( Q3ListViewItem*, const QPoint&, int ) ), SLOT( OnRequestPopup( Q3ListViewItem*, const QPoint& ) ) );
    return box;
}

// -----------------------------------------------------------------------------
// Name: ListParameter::OnRequestPopup
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
void ListParameter::OnRequestPopup( Q3ListViewItem* item, const QPoint& pos )
{
    Q3PopupMenu* menu = new Q3PopupMenu( list_ );
    if( createEnabled_ )
        menu->insertItem( tools::translate( "ListParameter", "Add" ), this, SLOT( OnCreate() ) );
    if( item )
        menu->insertItem( tools::translate( "ListParameter", "Remove" ), this, SLOT( OnDeleteSelectedItem() ) );
    menu->insertItem( tools::translate( "ListParameter", "Clear list" ), this, SLOT( OnClear() ) );
    menu->popup( pos );
}

// -----------------------------------------------------------------------------
// Name: ListParameter::EnableCreation
// Created: SBO 2007-05-02
// -----------------------------------------------------------------------------
void ListParameter::EnableCreation( bool enabled )
{
    createEnabled_ = enabled;
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
    Q3VBox* widget = new Q3VBox( list_->parentWidget() );
    widgets_[param] = widget;
    param->BuildInterface( widget );
    ::gui::ValuedListItem* item = new ::gui::ValuedListItem( list_, list_->lastItem() );
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
void ListParameter::OnSelectionChanged( Q3ListViewItem* item )
{
    if( selected_ == item )
        return;
    if( selected_ )
    {
        if( CheckValidity() )
        {
            if( Param_ABC* param = static_cast< ::gui::ValuedListItem* >( selected_ )->GetValue< Param_ABC >() )
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
        if( Param_ABC* param = static_cast< ::gui::ValuedListItem* >( item )->GetValue< Param_ABC >() )
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
void ListParameter::DeleteItem( Q3ListViewItem* item )
{
    if( item == selected_ )
    {
        if( Param_ABC* param = static_cast< ::gui::ValuedListItem* >( selected_ )->GetValue< Param_ABC >() )
        {
            param->RemoveFromController();
            widgets_[param]->hide();
        }
        selected_->setSelected( false );
        selected_ = 0;
    }
    if( Param_ABC* param = static_cast< ::gui::ValuedListItem* >( item )->GetValue< Param_ABC >() )
        DeleteElement( *param );
    delete item;
}

// -----------------------------------------------------------------------------
// Name: ListParameter::DeleteElement
// Created: SBO 2007-05-02
// -----------------------------------------------------------------------------
void ListParameter::DeleteElement( Param_ABC& param )
{
    widgets_.erase( &param );
    delete &param;
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
    Q3ListViewItemIterator it( list_ );
    for( unsigned int i = 0; it.current(); ++it, ++i )
    {
        ::gui::ValuedListItem* item = static_cast< ::gui::ValuedListItem* >( it.current() );
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
// Name: ListParameter::CommitChildrenTo
// Created: AGE 2007-07-11
// -----------------------------------------------------------------------------
void ListParameter::CommitChildrenTo( actions::ParameterContainer_ABC& parent ) const
{
    if( !list_ )
        return;
    for( Q3ListViewItemIterator it = Q3ListViewItemIterator( list_ ); it.current(); ++it )
    {
        ::gui::ValuedListItem* item = static_cast< ::gui::ValuedListItem* >( it.current() );
        item->GetValue< Param_ABC >()->CommitTo( parent );
    }
}

// -----------------------------------------------------------------------------
// Name: ListParameter::Select
// Created: SBO 2007-05-02
// -----------------------------------------------------------------------------
void ListParameter::Select( const Param_ABC& param )
{
    ::gui::ValuedListItem* item = ::gui::FindItem( &param, list_->firstChild() );
    if( item )
        list_->setSelected( item, true );
}

// -----------------------------------------------------------------------------
// Name: ListParameter::Invalid
// Created: SBO 2007-04-27
// -----------------------------------------------------------------------------
bool ListParameter::Invalid()
{
    if( list_ )
        list_->header()->setPaletteForegroundColor( Qt::red );
    QTimer::singleShot( 3000, this, SLOT( TurnHeaderBlack() ) );
    return false;
}

// -----------------------------------------------------------------------------
// Name: ListParameter::TurnHeaderBlack
// Created: SBO 2007-04-27
// -----------------------------------------------------------------------------
void ListParameter::TurnHeaderBlack()
{
    if( list_ )
        list_->header()->setPaletteForegroundColor( Qt::black );
}

// -----------------------------------------------------------------------------
// Name: ListParameter::IsOptional
// Created: SBO 2008-03-10
// -----------------------------------------------------------------------------
bool ListParameter::IsOptional() const
{
    return optional_;
}

// -----------------------------------------------------------------------------
// Name: ListParameter::SetLabel
// Created: ABR 2011-01-21
// -----------------------------------------------------------------------------
void ListParameter::SetLabel( const QString& label )
{
    if( list_ )
    {
        assert( list_->columns() == 1 );
        list_->setColumnText( 0, label );
    }
}
