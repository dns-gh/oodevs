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
#include <core/Model.h>

using namespace sword;

// -----------------------------------------------------------------------------
// Name: ListenerHelper::function< void
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
ListenerHelper::ListenerHelper( core::Model& model, T_Callback callback )
    : model_   ( &model )
    , callback_( callback )
{
    model.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ListenerHelper destructor
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
ListenerHelper::~ListenerHelper()
{
    if( model_ )
        model_->Unregister( *this );
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
    callback_( model );
}

// -----------------------------------------------------------------------------
// Name: ListenerHelper::NotifyRemoved
// Created: SLI 2012-07-16
// -----------------------------------------------------------------------------
void ListenerHelper::NotifyRemoved( const core::Model& /*model*/ )
{
    model_ = 0;
}
