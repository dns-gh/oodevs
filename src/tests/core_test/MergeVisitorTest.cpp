// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "core_test_pch.h"
#include "core/Model.h"
#include "MockModelVisitor.h"
#include "MockModelListener.h"
#include <boost/assign/list_of.hpp>

namespace
{
    struct Fixture
    {
        core::Model merger;
        core::Model mergee;
        core::MockModelListener listener;
    };
}

BOOST_FIXTURE_TEST_CASE( empty_model_merges_nothing_with_another_model, Fixture )
{
    merger.Merge( mergee, listener );
}

namespace
{
    void VisitNamedChild( const std::string& /*key*/, const core::Model& model, core::ModelVisitor_ABC& visitor )
    {
        model.Accept( visitor );
    }
    void VisitElements( const core::Elements& src, core::ModelVisitor_ABC& visitor, std::size_t expected )
    {
        BOOST_REQUIRE_EQUAL( src.Size(), expected );
        for( std::size_t i = 0; i < src.Size(); ++i )
            src[ i ].Accept( visitor );
    }
}

BOOST_FIXTURE_TEST_CASE( model_can_be_merged_with_another_model, Fixture )
{
    merger[ "int" ] = 3;
    merger[ "real" ] = 42.f;
    merger[ "elements" ].AddElement() = 6;
    merger[ "elements" ].AddElement() = 7;
    MOCK_EXPECT( listener.NotifyChanged ).exactly( 3 );
    merger.Merge( mergee, listener );

    core::MockModelVisitor visitor;
    MOCK_EXPECT( visitor.VisitNamedChild ).once().with( "int", mock::any ).calls( boost::bind( &core::Model::Accept, _2, boost::ref( visitor ) ) );
    MOCK_EXPECT( visitor.VisitNamedChild ).once().with( "real", mock::any ).calls( boost::bind( &core::Model::Accept, _2, boost::ref( visitor ) ) );
    MOCK_EXPECT( visitor.VisitNamedChild ).once().with( "elements", mock::any ).calls( boost::bind( &core::Model::Accept, _2, boost::ref( visitor ) ) );
    MOCK_EXPECT( visitor.VisitElement ).exactly( 2 ).calls( boost::bind( &core::Model::Accept, _1, boost::ref( visitor ) ) );
    MOCK_EXPECT( visitor.VisitInt64 ).once().with( 6 );
    MOCK_EXPECT( visitor.VisitInt64 ).once().with( 7 );
    MOCK_EXPECT( visitor.VisitInt64 ).once().with( 3 );
    MOCK_EXPECT( visitor.VisitDouble ).once().with( 42.f );
    mergee.Accept( visitor );
}

BOOST_FIXTURE_TEST_CASE( merging_too_few_elements_hides_mergee_extra_elements, Fixture )
{
    merger.AddElement();
    mergee.AddElement();
    core::Model& element1 = mergee.AddElement();
    core::Model& element2 = mergee.AddElement();
    MOCK_EXPECT( listener.NotifyRemoved ).once().with( mock::same( element1 ) );
    MOCK_EXPECT( listener.NotifyRemoved ).once().with( mock::same( element2 ) );
    MOCK_EXPECT( listener.NotifyChanged ).once().with( mock::same( mergee ) );
    merger.Merge( mergee, listener );
    BOOST_CHECK_EQUAL( mergee.GetSize(), 1u );
}

BOOST_FIXTURE_TEST_CASE( merging_too_many_elements_adds_mergee_extra_elements, Fixture )
{
    mergee.AddElement();
    merger.AddElement();
    merger.AddElement();
    merger.AddElement();
    MOCK_EXPECT( listener.NotifyChanged ).once().with( mock::same( mergee ) );
    merger.Merge( mergee, listener );
    BOOST_CHECK_EQUAL( mergee.GetSize(), 3u );
}

BOOST_FIXTURE_TEST_CASE( merging_same_number_of_elements_does_not_change_mergee_size, Fixture )
{
    mergee.AddElement();
    mergee.AddElement();
    merger.AddElement();
    merger.AddElement();
    merger.Merge( mergee, listener );
    BOOST_CHECK_EQUAL( mergee.GetSize(), 2u );
}

BOOST_FIXTURE_TEST_CASE( merging_model_shares_user_data_ownership, Fixture )
{
    merger.SetUserData( 1337 );
    merger.Merge( mergee, listener );
    BOOST_CHECK( merger.GetData() );
    BOOST_CHECK_EQUAL( merger.GetData(), mergee.GetData() );
}

BOOST_FIXTURE_TEST_CASE( merging_model_keeps_mergee_user_data_if_merger_has_none, Fixture )
{
    mergee.SetUserData( 1337 );
    merger.Merge( mergee, listener );
    BOOST_CHECK( mergee.GetData() );
}

BOOST_FIXTURE_TEST_CASE( merging_model_removes_elements_if_merger_has_none, Fixture )
{
    core::Model& element = mergee.AddElement();
    MOCK_EXPECT( listener.NotifyRemoved ).once().with( mock::same( element ) );
    MOCK_EXPECT( listener.NotifyChanged ).once().with( mock::same( mergee ) );
    merger.Merge( mergee, listener );
    BOOST_CHECK_EQUAL( mergee.GetSize(), 0u );
    core::MockModelVisitor visitor;
    mergee.Accept( visitor );
}

BOOST_FIXTURE_TEST_CASE( merging_link_throws, Fixture )
{
    core::Model& child = merger[ "child" ];
    core::Model& link = merger[ "link" ];
    link.Link( child );
    BOOST_CHECK_THROW( merger.Merge( mergee, listener ), std::exception );
}
