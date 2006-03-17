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
void SelectionProxy::Register( SelectionLayer_ABC& layer )
{
    layers_.push_back( &layer );
    if( layers_.size() == 1 )
    {
        focused_ = 0;
        layers_[ focused_ ]->SetFocus( true );
    }
}

// -----------------------------------------------------------------------------
// Name: SelectionProxy::Remove
// Created: SBO 2006-03-16
// -----------------------------------------------------------------------------
void SelectionProxy::Remove( SelectionLayer_ABC& layer )
{
    NotifyFocusLost( layer );
    layers_.erase( std::find( layers_.begin(), layers_.end(), &layer ) );
}

// -----------------------------------------------------------------------------
// Name: SelectionProxy::NotifyFocusLost
// Created: SBO 2006-03-16
// -----------------------------------------------------------------------------
void SelectionProxy::NotifyFocusLost( SelectionLayer_ABC& layer )
{
    if( &layer == layers_[ focused_ ] )
    {
        layers_[ focused_ ]->SetFocus( false );
        if( ++focused_ == layers_.size() )
            focused_ = 0;
        layers_[ focused_ ]->SetFocus( true );
    }
}
