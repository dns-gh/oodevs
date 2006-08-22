// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "SymbolRequest.h"
#include "AgentType.h"
#include "AgentNature.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: SymbolRequest constructor
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
SymbolRequest::SymbolRequest( const AgentNature& nature )
    : nature_( nature )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: SymbolRequest destructor
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
SymbolRequest::~SymbolRequest()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SymbolRequest::Matches
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
bool SymbolRequest::Matches( const std::string& name, const std::string& value ) const
{
    return nature_.Retrieve( name ) == value;
}
