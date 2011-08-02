// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "EntitySearchBox_ABC.h"
#include "moc_EntitySearchBox_ABC.cpp"
#include "EntitySearchItem.h"
#include "resources.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: EntitySearchBox_ABC constructor
// Created: AGE 2006-04-21
// -----------------------------------------------------------------------------
EntitySearchBox_ABC::EntitySearchBox_ABC( QWidget* parent, ActionController& actions )
    : Q3HBox( parent, "EntitySearchBox" )
    , actions_ ( actions )
    , lastItem_( items_.end() )
{
    setMargin( 2 );
    setSpacing( 10 );

    QLineEdit* edit = new QLineEdit( this );
    QPushButton* next = new QPushButton( QIcon( MAKE_ICON( search ) ), "", this );
    next->setAccel( Qt::Key_F3 );
    next->setMaximumWidth( edit->height() );
    next->setMaximumHeight( edit->height() );

    connect( edit, SIGNAL( textChanged( const QString& ) ), this, SLOT( Search( const QString& ) ) );
    connect( next, SIGNAL( pressed() ), this, SLOT( FindNext() ) );
}

// -----------------------------------------------------------------------------
// Name: EntitySearchBox_ABC destructor
// Created: AGE 2006-04-21
// -----------------------------------------------------------------------------
EntitySearchBox_ABC::~EntitySearchBox_ABC()
{
    for( CIT_Items it = items_.begin(); it != items_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: EntitySearchBox_ABC::AddItem
// Created: AGE 2006-04-21
// -----------------------------------------------------------------------------
void EntitySearchBox_ABC::AddItem( const Entity_ABC& entity )
{
    items_.push_back( new EntitySearchItem( actions_, entity ) );
    lastItem_ = items_.begin();
}

// -----------------------------------------------------------------------------
// Name: EntitySearchBox_ABC::RemoveItem
// Created: AGE 2006-04-21
// -----------------------------------------------------------------------------
void EntitySearchBox_ABC::RemoveItem( const Entity_ABC& entity )
{
    for( IT_Items it = items_.begin(); it != items_.end(); ++it )
    {
        if( (*it)->Matches( entity ) )
        {
            std::swap( *it, items_.back() );
            delete items_.back();
            items_.pop_back();
            lastItem_ = items_.begin();
            return;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: EntitySearchBox_ABC::Search
// Created: AGE 2006-04-21
// -----------------------------------------------------------------------------
void EntitySearchBox_ABC::Search( const QString& input )
{
    currentSearch_ = input.lower();
    Find();
}

// -----------------------------------------------------------------------------
// Name: EntitySearchBox_ABC::Find
// Created: AGE 2006-04-21
// -----------------------------------------------------------------------------
bool EntitySearchBox_ABC::Find( CIT_Items begin, CIT_Items end )
{
    if( currentSearch_.isEmpty() )
        return false;
    for( CIT_Items it = begin; it < end; ++it )
    {
        if( (*it)->Matches( currentSearch_ ) )
        {
            (*it)->Activate();
            lastItem_ = it;
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: EntitySearchBox_ABC::Find
// Created: AGE 2006-04-21
// -----------------------------------------------------------------------------
void EntitySearchBox_ABC::Find()
{
    if( ! Find( lastItem_, items_.end() ) )
        Find( items_.begin(), lastItem_ );
}

// -----------------------------------------------------------------------------
// Name: EntitySearchBox_ABC::FindNext
// Created: AGE 2006-04-21
// -----------------------------------------------------------------------------
void EntitySearchBox_ABC::FindNext()
{
    if( lastItem_ == items_.end() || ++lastItem_ == items_.end() )
        lastItem_ = items_.begin();
    Find();
}
