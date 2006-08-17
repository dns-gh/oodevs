// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gaming_pch.h"
#include "DotationRequest.h"

// -----------------------------------------------------------------------------
// Name: DotationRequest constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
DotationRequest::DotationRequest( const DotationType& type, unsigned int requested, unsigned int granted, unsigned int convoyed )
    : type_( &type )
    , requested_( requested )
    , granted_( granted )
    , convoyed_( convoyed )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DotationRequest destructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
DotationRequest::~DotationRequest()
{
    // NOTHING
}
