// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "core_test_pch.h"
#include "core/Model.h"
#include "core/NamedChildrenVisitor.h"

namespace
{
    MOCK_FUNCTION( NamedChildrenVisitor, 2, void( const std::string& key, const core::Model& child ), NamedChildrenVisitor );
}

BOOST_AUTO_TEST_CASE( named_children_visitor_notifies_functor_with_every_child )
{
    core::Model model;
    const core::Model& child_1 = model[ "child_1" ];
    const core::Model& child_2 = model[ "child_2" ];
    MOCK_EXPECT( NamedChildrenVisitor ).once().with( "child_1", mock::same( child_1 ) );
    MOCK_EXPECT( NamedChildrenVisitor ).once().with( "child_2", mock::same( child_2 ) );
    core::NamedChildrenVisitor( model, &NamedChildrenVisitor );
}
