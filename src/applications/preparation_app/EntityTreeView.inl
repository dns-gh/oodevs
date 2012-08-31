// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: EntityTreeView constructor
// Created: ABR 2012-08-14
// -----------------------------------------------------------------------------
template< typename Entity >
EntityTreeView< Entity >::EntityTreeView( QWidget* parent /* = 0 */ )
    : gui::EntityTreeView< Entity >( parent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView destructor
// Created: ABR 2012-08-14
// -----------------------------------------------------------------------------
template< typename Entity >
EntityTreeView< Entity >::~EntityTreeView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView::contextMenuEvent
// Created: ABR 2012-08-14
// -----------------------------------------------------------------------------
template< typename Entity >
inline
void EntityTreeView< Entity >::contextMenuEvent( QContextMenuEvent* e )
{
    // $$$$ ABR 2012-08-14: TODO
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView::keyPressEvent
// Created: ABR 2012-08-14
// -----------------------------------------------------------------------------
template< typename Entity >
inline
void EntityTreeView< Entity >::keyPressEvent( QKeyEvent* e )
{
    // $$$$ ABR 2012-08-14: TODO
}
