// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "NatureSelectionWidget.h"
#include "moc_NatureSelectionWidget.cpp"
#include "clients_kernel/SymbolRule.h"
#include "xeumeuleu/xml.h"

using namespace xml;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: NatureSelectionWidget constructor
// Created: SBO 2007-10-12
// -----------------------------------------------------------------------------
NatureSelectionWidget::NatureSelectionWidget( QWidget* parent, const std::string& symbolFile )
    : QListView( parent )
    , category_( 0 )
    , current_( 0 )
{
    addColumn( "" );
    header()->hide();
    setRootIsDecorated( true );
    setResizeMode( QListView::ResizeMode::LastColumn );
    xifstream xis( symbolFile );
    xis >> start( "app6" )
            >> start( "symbols" )
                >> start( "choice" );
    kernel::SymbolRule root( xis );
    root.Accept( *this );
    connect( this, SIGNAL( selectionChanged( QListViewItem* ) ), SLOT( OnSelectionChanged( QListViewItem* ) ) );
}

// -----------------------------------------------------------------------------
// Name: NatureSelectionWidget destructor
// Created: SBO 2007-10-12
// -----------------------------------------------------------------------------
NatureSelectionWidget::~NatureSelectionWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NatureSelectionWidget::StartCategory
// Created: SBO 2007-10-12
// -----------------------------------------------------------------------------
void NatureSelectionWidget::StartCategory( const std::string& title )
{
    category_ = current_;
}

// -----------------------------------------------------------------------------
// Name: NatureSelectionWidget::AddChoice
// Created: SBO 2007-10-12
// -----------------------------------------------------------------------------
void NatureSelectionWidget::AddChoice( kernel::SymbolRule* rule, const std::string& name, const std::string& value )
{
    if( !category_ )
        current_ = new QListViewItem( this );
    else
        current_ = new QListViewItem( category_ );
    current_->setText( 0, name.c_str() );
    current_->setDragEnabled( true );
    if( rule )
    {
        QListViewItem* category = category_;
        rule->Accept( *this );
        category_ = category;
    }
}

// -----------------------------------------------------------------------------
// Name: NatureSelectionWidget::OnSelectionChanged
// Created: SBO 2007-10-12
// -----------------------------------------------------------------------------
void NatureSelectionWidget::OnSelectionChanged( QListViewItem* item )
{
    QStringList list;
    while( item )
    {
        list.push_front( item->text( 0 ) );
        item = item->parent();
    }
    const QString nature = list.join( "/" );
    emit NatureSelected( nature );
}

// -----------------------------------------------------------------------------
// Name: NatureSelectionWidget::startDrag
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
void NatureSelectionWidget::startDrag()
{
    emit StartDrag();
}
