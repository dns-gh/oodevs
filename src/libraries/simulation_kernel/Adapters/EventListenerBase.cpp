// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "EventListenerBase.h"
#include <core/Facade.h>

using namespace sword;

// -----------------------------------------------------------------------------
// Name: EventListenerBase constructor
// Created: MCO 2013-01-14
// -----------------------------------------------------------------------------
EventListenerBase::EventListenerBase( core::Facade& facade, const std::string& event )
    : facade_( facade )
    , event_ ( event )
{
    facade.Register( event, *this );
}

// -----------------------------------------------------------------------------
// Name: EventListenerBase destructor
// Created: MCO 2013-01-14
// -----------------------------------------------------------------------------
EventListenerBase::~EventListenerBase()
{
    facade_.Unregister( event_, *this );
}
