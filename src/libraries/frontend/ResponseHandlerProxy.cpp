// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "ResponseHandlerProxy.h"

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: ResponseHandlerProxy constructor
// Created: SBO 2010-11-22
// -----------------------------------------------------------------------------
ResponseHandlerProxy::ResponseHandlerProxy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResponseHandlerProxy destructor
// Created: SBO 2010-11-22
// -----------------------------------------------------------------------------
ResponseHandlerProxy::~ResponseHandlerProxy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResponseHandlerProxy::SetMainHandler
// Created: SBO 2010-11-22
// -----------------------------------------------------------------------------
void ResponseHandlerProxy::SetMainHandler( boost::shared_ptr< ResponseHandler_ABC > handler )
{
    mainHandler_ = handler;
}

// -----------------------------------------------------------------------------
// Name: ResponseHandlerProxy::ResetMainHandler
// Created: SBO 2010-11-22
// -----------------------------------------------------------------------------
void ResponseHandlerProxy::ResetMainHandler()
{
    mainHandler_.reset();
}

// -----------------------------------------------------------------------------
// Name: ResponseHandlerProxy::Register
// Created: SBO 2010-11-22
// -----------------------------------------------------------------------------
void ResponseHandlerProxy::RegisterLauncherHandler( boost::shared_ptr< ResponseHandler_ABC > handler )
{
    handlers_.push_back( handler );
}

// -----------------------------------------------------------------------------
// Name: ResponseHandlerProxy::Handle
// Created: SBO 2010-11-22
// -----------------------------------------------------------------------------
void ResponseHandlerProxy::Handle( const sword::ExerciseListResponse& message )
{
    UpdateHandlers( message );
}

// -----------------------------------------------------------------------------
// Name: ResponseHandlerProxy::UpdateHandlers
// Created: SBO 2010-11-22
// -----------------------------------------------------------------------------
template< typename Message >
void ResponseHandlerProxy::UpdateHandlers( const Message& message )
{
    if( mainHandler_.get() )
        mainHandler_->Handle( message );
    for( std::vector< boost::shared_ptr< ResponseHandler_ABC > >::const_iterator it = handlers_.begin(); it != handlers_.end(); ++it )
        (*it)->Handle( message );
}
