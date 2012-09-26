// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "ZOPerceptionComputer.h"
#include "PerceptionLevel.h"
#include "PerceptionComputer.h"
#include "PerceptionSurfaceAgent.h"
#include "SurfacesAgentVisitor_ABC.h"
#include "SurfacesAgent_ABC.h"
#include <boost/bind.hpp>

using namespace sword;
using namespace sword::perception;

// -----------------------------------------------------------------------------
// Name: ZOPerceptionComputer constructor
// Created: SLG 2010-04-29
// -----------------------------------------------------------------------------
ZOPerceptionComputer::ZOPerceptionComputer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ZOPerceptionComputer destructor
// Created: SLG 2010-04-29
// -----------------------------------------------------------------------------
ZOPerceptionComputer::~ZOPerceptionComputer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ZOPerceptionComputer::ComputePerception
// Created: SLG 2010-04-29
// -----------------------------------------------------------------------------
const PerceptionLevel& ZOPerceptionComputer::ComputePerception( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const wrapper::View& target ) const
{
    PerceptionComputer< SurfacesAgentVisitor_ABC, PerceptionSurfaceAgent > computer( boost::bind( &PerceptionSurfaceAgent::ComputeAgentPerception, _1, boost::cref( perceiver ), boost::cref( target ) ) );
    surfaces.Apply( computer );
    return computer.GetLevel();
}
