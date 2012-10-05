// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "InputPropagationAttribute.h"
#include "MIL_AgentServer.h"

BOOST_CLASS_EXPORT_IMPLEMENT( InputPropagationAttribute )


// -----------------------------------------------------------------------------
// Name: InputPropagationAttribute constructor
// Created: LGY 2012-10-05
// -----------------------------------------------------------------------------
InputPropagationAttribute::InputPropagationAttribute()
{
    // NOTHING
}

namespace
{
    std::string BuildPropagationFile( const std::string& path )
    {
        return MIL_AgentServer::GetWorkspace().GetConfig().GetPropagationFile( path );
    }
}

// -----------------------------------------------------------------------------
// Name: InputPropagationAttribute constructor
// Created: LGY 2012-10-05
// -----------------------------------------------------------------------------
InputPropagationAttribute::InputPropagationAttribute( xml::xistream& xis )
    :  model_( BuildPropagationFile( xis.attribute< std::string >( "source" ) ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InputPropagationAttribute destructor
// Created: LGY 2012-10-05
// -----------------------------------------------------------------------------
InputPropagationAttribute::~InputPropagationAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InputPropagationAttribute::serialize
// Created: LGY 2012-10-05
// -----------------------------------------------------------------------------
template< typename Archive >
void InputPropagationAttribute::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectAttribute_ABC >( *this )
         & model_;
}
