// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "ModelCounter.h"
#include <core/Model.h>

using namespace sword;

// -----------------------------------------------------------------------------
// Name: ModelCounter constructor
// Created: MCO 2012-09-28
// -----------------------------------------------------------------------------
ModelCounter::ModelCounter()
    : count_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModelCounter destructor
// Created: MCO 2012-09-28
// -----------------------------------------------------------------------------
ModelCounter::~ModelCounter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModelCounter::Visit
// Created: MCO 2012-09-28
// -----------------------------------------------------------------------------
void ModelCounter::Visit( int64_t /*value*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModelCounter::Visit
// Created: MCO 2012-09-28
// -----------------------------------------------------------------------------
void ModelCounter::Visit( uint64_t /*value*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModelCounter::Visit
// Created: MCO 2012-09-28
// -----------------------------------------------------------------------------
void ModelCounter::Visit( double /*value*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModelCounter::Visit
// Created: MCO 2012-09-28
// -----------------------------------------------------------------------------
void ModelCounter::Visit( const std::string& /*value*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModelCounter::Visit
// Created: MCO 2012-09-28
// -----------------------------------------------------------------------------
void ModelCounter::Visit( const std::string& /*key*/, const core::Model& child )
{
    ++count_;
    child.Accept( *this );
}

// -----------------------------------------------------------------------------
// Name: ModelCounter::Visit
// Created: MCO 2012-09-28
// -----------------------------------------------------------------------------
void ModelCounter::Visit( std::size_t /*key*/, const core::Model& child )
{
    ++count_;
    child.Accept( *this );
}

// -----------------------------------------------------------------------------
// Name: ModelCounter::Visit
// Created: MCO 2012-09-28
// -----------------------------------------------------------------------------
void ModelCounter::Visit( const core::Model& element )
{
    ++count_;
    element.Accept( *this );
}

// -----------------------------------------------------------------------------
// Name: ModelCounter::Visit
// Created: MCO 2012-09-28
// -----------------------------------------------------------------------------
void ModelCounter::Visit( const boost::shared_ptr< core::UserData_ABC >& /*data*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModelCounter::MarkForRemove
// Created: MCO 2012-09-28
// -----------------------------------------------------------------------------
void ModelCounter::MarkForRemove()
{
    // NOTHING
}
