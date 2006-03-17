// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "SelectionProxy.h"
#include "SelectionLayer_ABC.h"

// -----------------------------------------------------------------------------
// Name: SelectionProxy constructor
// Created: SBO 2006-03-16
// -----------------------------------------------------------------------------
SelectionProxy::SelectionProxy()
    : last_( 0 )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: SelectionProxy destructor
// Created: SBO 2006-03-16
// -----------------------------------------------------------------------------
SelectionProxy::~SelectionProxy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SelectionProxy::Register
// Created: SBO 2006-03-16
// -----------------------------------------------------------------------------
void SelectionProxy::Register( SelectionLayer_ABC& layer, int position /*=-1*/)
{
    if( position == -1 )
        position = last_++;

    if( ! layers_.insert( std::make_pair( position, &layer ) ).second )
        throw std::runtime_error( "Layer index already in use" );
    if( layers_.size() == 1 )
    {
        focused_ = layers_.begin();
        focused_->second->SetFocus( true );
    }
}

// -----------------------------------------------------------------------------
// Name: SelectionProxy::Remove
// Created: SBO 2006-03-16
// -----------------------------------------------------------------------------
void SelectionProxy::Remove( SelectionLayer_ABC& layer )
{
    NotifyFocusLost( layer );
    for( IT_Layers it = layers_.begin(); it != layers_.end(); ++it )
        if( it->second == &layer )
            layers_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: SelectionProxy::NotifyFocusLost
// Created: SBO 2006-03-16
// -----------------------------------------------------------------------------
void SelectionProxy::NotifyFocusLost( SelectionLayer_ABC& layer )
{
    if( &layer == focused_->second )
    {
        focused_->second->SetFocus( false );
        if( ++focused_ == layers_.end() )
            focused_ = layers_.begin();
        focused_->second->SetFocus( true );
    }
}
