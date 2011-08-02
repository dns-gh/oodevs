// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "SmartGridWidget.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: SmartGridWidget constructor
// Created: AGE 2006-06-23
// -----------------------------------------------------------------------------
SmartGridWidget::SmartGridWidget( QWidget* parent, int strips, Qt::Orientation o, const char* name )
    : QWidget( parent, name )
    , strips_    ( strips )
    , o_         ( o )
    , childCount_( 0 )
{
    if( strips_ <= 0 )
        strips_ = 1;
    layout_ = new Q3GridLayout( this );
    layout_->setSpacing( 4 );
}

// -----------------------------------------------------------------------------
// Name: SmartGridWidget destructor
// Created: AGE 2006-06-23
// -----------------------------------------------------------------------------
SmartGridWidget::~SmartGridWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SmartGridWidget::childEvent
// Created: AGE 2006-06-23
// -----------------------------------------------------------------------------
void SmartGridWidget::childEvent( QChildEvent * e )
{
    if( e && e->child() && e->child()->inherits( "QWidget" ) && e->inserted() )
        AddChild( (QWidget*)e->child() );
    else if( e->removed() )
        Reorganize();
    QWidget::childEvent( e );
}

// -----------------------------------------------------------------------------
// Name: SmartGridWidget::AddChild
// Created: AGE 2006-06-23
// -----------------------------------------------------------------------------
void SmartGridWidget::AddChild( QWidget* child )
{
    unsigned int row = childCount_ % strips_;
    unsigned int col = childCount_ / strips_;
    if( o_ == Qt::Vertical )
        std::swap( row, col );
    layout_->addWidget( child, row, col );
    ++childCount_;
}

// -----------------------------------------------------------------------------
// Name: SmartGridWidget::Reorganize
// Created: AGE 2006-06-23
// -----------------------------------------------------------------------------
void SmartGridWidget::Reorganize()
{
    std::vector< QWidget* > widgets;
    widgets.reserve( childCount_ );
    QLayoutIterator it = layout_->iterator();
    for( QLayoutItem* item = it.current(); item; item = it.current() )
        if( QWidget* widget = item->widget() )
        {
            widgets.push_back( widget );
            it.takeCurrent();
        }
        else
            ++it;
    childCount_ = 0;
    for( std::vector< QWidget* >::iterator it = widgets.begin(); it != widgets.end(); ++it )
        AddChild( *it );
}
