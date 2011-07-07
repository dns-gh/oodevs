// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "AutomatComposition.h"
#include "tools/Resolver_ABC.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AutomatComposition constructor
// Created: AGE 2006-09-05
// -----------------------------------------------------------------------------
AutomatComposition::AutomatComposition( xml::xistream& input, const tools::Resolver_ABC< AgentType, std::string >& agentResolver )
    : type_( agentResolver.Get( input.attribute< std::string >( "type" ) ) )
    , min_( 0 )
    , max_( std::numeric_limits< unsigned >::max() )
{
    input >> xml::optional >> xml::attribute( "min-occurs", min_ )
          >> xml::optional >> xml::attribute( "max-occurs", max_ );
    if( min_ > max_ )
        std::swap( min_, max_ );
}

// -----------------------------------------------------------------------------
// Name: AutomatComposition destructor
// Created: AGE 2006-09-05
// -----------------------------------------------------------------------------
AutomatComposition::~AutomatComposition()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatComposition::GetType
// Created: AGE 2006-09-05
// -----------------------------------------------------------------------------
const AgentType& AutomatComposition::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: AutomatComposition::GetMin
// Created: AGE 2006-09-05
// -----------------------------------------------------------------------------
unsigned int AutomatComposition::GetMin() const
{
    return min_;
}

// -----------------------------------------------------------------------------
// Name: AutomatComposition::GetMax
// Created: AGE 2006-09-05
// -----------------------------------------------------------------------------
unsigned int AutomatComposition::GetMax() const
{
    return max_;
}

// -----------------------------------------------------------------------------
// Name: AutomatComposition::GetSensibleNumber
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
unsigned int AutomatComposition::GetSensibleNumber() const
{
    if( max_ == min_ )
        return min_;
    if( max_ != std::numeric_limits< unsigned int >::max() )
        return std::max( ( max_ - min_ ) / 2u, 1u );
    return std::max( min_, 1u );
}
