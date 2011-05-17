// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "ModelFunctionComposite.h"
#include <boost/bind.hpp>

// -----------------------------------------------------------------------------
// Name: ModelFunctionComposite constructor
// Created: AGE 2007-08-30
// -----------------------------------------------------------------------------
ModelFunctionComposite::ModelFunctionComposite()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModelFunctionComposite destructor
// Created: AGE 2007-08-30
// -----------------------------------------------------------------------------
ModelFunctionComposite::~ModelFunctionComposite()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModelFunctionComposite::Add
// Created: AGE 2007-08-30
// -----------------------------------------------------------------------------
void ModelFunctionComposite::Add( boost::shared_ptr< ModelFunction_ABC > function )
{
    functions_.push_back( function );
}

// -----------------------------------------------------------------------------
// Name: ModelFunctionComposite::BeginTick
// Created: AGE 2007-08-30
// -----------------------------------------------------------------------------
void ModelFunctionComposite::BeginTick()
{
    std::for_each( functions_.begin(), functions_.end(), boost::bind( &ModelFunction_ABC::BeginTick, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: ModelFunctionComposite::Receive
// Created: AGE 2007-08-30
// -----------------------------------------------------------------------------
void ModelFunctionComposite::Receive( const sword::SimToClient& message )
{
    std::for_each( functions_.begin(), functions_.end(), boost::bind( &ModelFunction_ABC::Receive, _1, boost::cref( message ) ) );
}

// -----------------------------------------------------------------------------
// Name: ModelFunctionComposite::EndTick
// Created: AGE 2007-08-30
// -----------------------------------------------------------------------------
void ModelFunctionComposite::EndTick()
{
    std::for_each( functions_.begin(), functions_.end(), boost::bind( &ModelFunction_ABC::EndTick, _1 ) );
}
