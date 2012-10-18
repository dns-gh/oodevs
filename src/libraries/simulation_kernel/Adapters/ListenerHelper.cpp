// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "ListenerHelper.h"
#include "Sink.h"
#include <core/Model.h>

using namespace sword;

// -----------------------------------------------------------------------------
// Name: ListenerHelper constructor
// Created: SLI 2012-07-24
// -----------------------------------------------------------------------------
ListenerHelper::ListenerHelper( Sink& sink, const core::Model& model, T_Callback changed )
    : sink_   ( sink )
    , model_  ( &model )
    , changed_( changed )
{
    sink_.Register( *model_, *this );
}

// -----------------------------------------------------------------------------
// Name: ListenerHelper constructor
// Created: SLI 2012-07-24
// -----------------------------------------------------------------------------
ListenerHelper::ListenerHelper( Sink& sink, const core::Model& model, T_Callback changed, T_Callback removed )
    : sink_   ( sink )
    , model_  ( &model )
    , changed_( changed )
    , removed_( removed )
{
    sink_.Register( *model_, *this );
}

// -----------------------------------------------------------------------------
// Name: ListenerHelper destructor
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
ListenerHelper::~ListenerHelper()
{
    if( model_ )
        sink_.Unregister( *model_, *this );
}

// -----------------------------------------------------------------------------
// Name: ListenerHelper::operator*
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
const core::Model& ListenerHelper::operator*() const
{
    if( !model_)
        throw std::runtime_error( "model has been removed" );
    return *model_;
}

// -----------------------------------------------------------------------------
// Name: ListenerHelper::NotifyChanged
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void ListenerHelper::NotifyChanged( const core::Model& model )
{
    model_ = const_cast< core::Model* >( &model );
    if( changed_ )
        changed_( model );
}

// -----------------------------------------------------------------------------
// Name: ListenerHelper::NotifyRemoved
// Created: SLI 2012-07-16
// -----------------------------------------------------------------------------
void ListenerHelper::NotifyRemoved( const core::Model& model )
{
    if( removed_ )
        removed_( model );
    sink_.Unregister( *model_, *this );
    model_ = 0;
}
