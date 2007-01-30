// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "ExerciseList.h"
#include "frontend/Exercise.h"
#include "clients_gui/ValuedListItem.h"

// -----------------------------------------------------------------------------
// Name: ExerciseList constructor
// Created: SBO 2007-01-26
// -----------------------------------------------------------------------------
ExerciseList::ExerciseList( QWidget* parent )
    : QListView( parent )
{
    addColumn( tr( "Exercise" ) );
}

// -----------------------------------------------------------------------------
// Name: ExerciseList destructor
// Created: SBO 2007-01-26
// -----------------------------------------------------------------------------
ExerciseList::~ExerciseList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::NotifyCreated
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
void ExerciseList::NotifyCreated( const frontend::Exercise& exercise )
{
    gui::ValuedListItem* item = new gui::ValuedListItem( this );
    item->SetNamed( exercise );
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::NotifyUpdated
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
void ExerciseList::NotifyUpdated( const frontend::Exercise& exercise )
{
    if( gui::ValuedListItem* item = gui::FindItem( &exercise, firstChild() ) )
        item->SetNamed( exercise );
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::NotifyDeleted
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
void ExerciseList::NotifyDeleted( const frontend::Exercise& exercise )
{
    if( gui::ValuedListItem* item = gui::FindItem( &exercise, firstChild() ) )
        delete item;
}
