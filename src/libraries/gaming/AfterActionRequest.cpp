// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AfterActionRequest.h"
#include "AfterActionFunction.h"
#include "ASN_Messages.h"
#include "clients_kernel/Controller.h"

// -----------------------------------------------------------------------------
// Name: AfterActionRequest constructor
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
AfterActionRequest::AfterActionRequest( kernel::Controller& controller, const AfterActionFunction& function, Publisher_ABC& publisher )
    : controller_( controller )
    , function_( function )
{
    ASN_MsgIndicatorRequest request;
    const std::string xmlRequest = function.Commit();
    request().identifier = reinterpret_cast< int >( this );
    request().request = xmlRequest.c_str();
    request.Send( publisher );
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequest destructor
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
AfterActionRequest::~AfterActionRequest()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequest::Update
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
void AfterActionRequest::Update( const ASN1T_MsgIndicatorResult& message )
{
    if( message.identifier == reinterpret_cast< int >( this ) )
    {
        result_.resize( message.values.n );
        std::copy( message.values.elem, message.values.elem + message.values.n, result_.begin() );
        error_ = message.error;
        controller_.Update( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequest::GetName
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
QString AfterActionRequest::GetName() const
{
    return function_.GetName();
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequest::IsPending
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
bool AfterActionRequest::IsPending() const
{
    return result_.empty() && error_.empty();
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequest::IsDone
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
bool AfterActionRequest::IsDone() const
{
    return !result_.empty();
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequest::IsFailed
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
bool AfterActionRequest::IsFailed() const
{
    return !error_.empty();
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequest::ErrorMessage
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
QString AfterActionRequest::ErrorMessage() const
{
    return QString( error_.c_str() );
}
