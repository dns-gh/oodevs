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
    , publisher_( publisher )
    , done_( false )
{
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
// Name: AfterActionRequest::SetParameter
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
void AfterActionRequest::SetParameter( const std::string& name, const std::string& value )
{
    parameters_[ name ] = value;
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequest::Commit
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
void AfterActionRequest::Commit()
{
    ASN_MsgIndicatorRequest request;
    const std::string xmlRequest = function_.Commit( parameters_ );
    request().identifier = reinterpret_cast< int >( this );
    request().request = xmlRequest.c_str();
    request.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequest::Update
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
void AfterActionRequest::Update( const ASN1T_MsgIndicatorResult& message )
{
    if( message.identifier == reinterpret_cast< int >( this ) )
    {
        done_ = true;
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
    return !done_;
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequest::IsDone
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
bool AfterActionRequest::IsDone() const
{
    return done_ && !result_.empty();
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequest::IsFailed
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
bool AfterActionRequest::IsFailed() const
{
    return done_ && result_.empty();
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequest::ErrorMessage
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
QString AfterActionRequest::ErrorMessage() const
{
    return QString( error_.c_str() );
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequest::Result
// Created: AGE 2007-09-26
// -----------------------------------------------------------------------------
const AfterActionRequest::T_Data& AfterActionRequest::Result() const
{
    return result_;
}
