// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "IdentifiedChildrenVisitor.h"
#include "Model.h"

using namespace core;

// -----------------------------------------------------------------------------
// Name: IdentifiedChildrenVisitor constructor
// Created: SLI 2012-07-18
// -----------------------------------------------------------------------------
IdentifiedChildrenVisitor::IdentifiedChildrenVisitor( const Model& parent, const T_Functor& functor )
    : functor_( functor )
{
    parent.Accept( *this );
}

// -----------------------------------------------------------------------------
// Name: IdentifiedChildrenVisitor destructor
// Created: SLI 2012-07-18
// -----------------------------------------------------------------------------
IdentifiedChildrenVisitor::~IdentifiedChildrenVisitor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IdentifiedChildrenVisitor::Visit
// Created: SLI 2012-07-18
// -----------------------------------------------------------------------------
void IdentifiedChildrenVisitor::Visit( const std::string& /*key*/, const Model& /*child*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IdentifiedChildrenVisitor::Visit
// Created: MCO 2012-09-25
// -----------------------------------------------------------------------------
void IdentifiedChildrenVisitor::Visit( std::size_t key, const Model& child )
{
    functor_( key, child );
}

// -----------------------------------------------------------------------------
// Name: IdentifiedChildrenVisitor::Visit
// Created: SLI 2012-07-18
// -----------------------------------------------------------------------------
void IdentifiedChildrenVisitor::Visit( int64_t /*value*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IdentifiedChildrenVisitor::Visit
// Created: SLI 2012-07-18
// -----------------------------------------------------------------------------
void IdentifiedChildrenVisitor::Visit( uint64_t /*value*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IdentifiedChildrenVisitor::Visit
// Created: SLI 2012-07-18
// -----------------------------------------------------------------------------
void IdentifiedChildrenVisitor::Visit( double /*value*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IdentifiedChildrenVisitor::Visit
// Created: SLI 2012-07-18
// -----------------------------------------------------------------------------
void IdentifiedChildrenVisitor::Visit( const std::string& /*value*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IdentifiedChildrenVisitor::Visit
// Created: SLI 2012-07-18
// -----------------------------------------------------------------------------
void IdentifiedChildrenVisitor::Visit( const Model& /*element*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IdentifiedChildrenVisitor::Visit
// Created: SLI 2012-07-18
// -----------------------------------------------------------------------------
void IdentifiedChildrenVisitor::Visit( const boost::shared_ptr< UserData_ABC >& /*data*/ )
{
    // NOTHING
}
