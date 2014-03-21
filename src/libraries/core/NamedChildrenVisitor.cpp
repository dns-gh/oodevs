// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "NamedChildrenVisitor.h"
#include "Model.h"

using namespace core;

// -----------------------------------------------------------------------------
// Name: NamedChildrenVisitor constructor
// Created: SLI 2012-07-18
// -----------------------------------------------------------------------------
NamedChildrenVisitor::NamedChildrenVisitor( const Model& parent, const T_Functor& functor )
    : functor_( functor )
{
    parent.Accept( *this );
}

// -----------------------------------------------------------------------------
// Name: NamedChildrenVisitor destructor
// Created: SLI 2012-07-18
// -----------------------------------------------------------------------------
NamedChildrenVisitor::~NamedChildrenVisitor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NamedChildrenVisitor::Visit
// Created: SLI 2012-07-18
// -----------------------------------------------------------------------------
void NamedChildrenVisitor::Visit( const std::string& key, const Model& child )
{
    functor_( key, child );
}

// -----------------------------------------------------------------------------
// Name: NamedChildrenVisitor::Visit
// Created: MCO 2012-09-25
// -----------------------------------------------------------------------------
void NamedChildrenVisitor::Visit( std::size_t /*key*/, const Model& /*child*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NamedChildrenVisitor::Visit
// Created: SLI 2012-07-18
// -----------------------------------------------------------------------------
void NamedChildrenVisitor::Visit( int64_t /*value*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NamedChildrenVisitor::Visit
// Created: SLI 2012-07-18
// -----------------------------------------------------------------------------
void NamedChildrenVisitor::Visit( uint64_t /*value*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NamedChildrenVisitor::Visit
// Created: SLI 2012-07-18
// -----------------------------------------------------------------------------
void NamedChildrenVisitor::Visit( double /*value*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NamedChildrenVisitor::Visit
// Created: SLI 2012-07-18
// -----------------------------------------------------------------------------
void NamedChildrenVisitor::Visit( const std::string& /*value*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NamedChildrenVisitor::Visit
// Created: SLI 2012-07-18
// -----------------------------------------------------------------------------
void NamedChildrenVisitor::Visit( const Model& /*element*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NamedChildrenVisitor::Visit
// Created: SLI 2012-07-18
// -----------------------------------------------------------------------------
void NamedChildrenVisitor::Visit( const boost::shared_ptr< UserData_ABC >& /*data*/ )
{
    // NOTHING
}
