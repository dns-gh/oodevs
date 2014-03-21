// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "core_test_pch.h"
#include "core/MakeModel.h"
#include "MockModelVisitor.h"

namespace
{
    void ( core::Model::*accept )( core::ModelVisitor_ABC& ) const = &core::Model::Accept;

    struct Fixture
    {
        void Check( const core::Model& model )
        {
            model.Accept( visitor );
        }
        core::MockModelVisitor visitor;
    };
}

BOOST_FIXTURE_TEST_CASE( model_builder_can_build_an_empty_model, Fixture )
{
    Check( core::MakeModel() );
}

BOOST_FIXTURE_TEST_CASE( model_builder_can_build_data_model, Fixture)
{
    MOCK_EXPECT( visitor.VisitInt64 ).once().with( 42 );
    Check( core::MakeModel( 42 ) );
}

BOOST_FIXTURE_TEST_CASE( model_builder_can_build_child_value_model, Fixture)
{
    mock::sequence s;
    MOCK_EXPECT( visitor.VisitNamedChild ).once().in( s ).with( "child", mock::any ).calls( boost::bind( accept, _2, boost::ref( visitor ) ) );
    MOCK_EXPECT( visitor.VisitInt64 ).once().in( s ).with( 42 );
    Check( core::MakeModel( "child", 42 ) );
}

BOOST_FIXTURE_TEST_CASE( model_builder_can_chain_child_value_model, Fixture)
{
    mock::sequence s;
    MOCK_EXPECT( visitor.VisitNamedChild ).once().in( s ).with( "child", mock::any ).calls( boost::bind( accept, _2, boost::ref( visitor ) ) );
    MOCK_EXPECT( visitor.VisitInt64 ).once().in( s ).with( 42 );
    MOCK_EXPECT( visitor.VisitNamedChild ).once().in( s ).with( "child 2", mock::any ).calls( boost::bind( accept, _2, boost::ref( visitor ) ) );
    MOCK_EXPECT( visitor.VisitString ).once().in( s ).with( "some string" );
    Check( core::MakeModel( "child", 42 )( "child 2", "some string" ) );
}

BOOST_FIXTURE_TEST_CASE( model_builder_can_build_empty_child_model, Fixture)
{
    mock::sequence s;
    MOCK_EXPECT( visitor.VisitNamedChild ).once().in( s ).with( "child", mock::any ).calls( boost::bind( accept, _2, boost::ref( visitor ) ) );
    Check( core::MakeModel( "child", core::MakeModel() ) );
}

BOOST_FIXTURE_TEST_CASE( model_builder_can_chain_empty_child_model, Fixture)
{
    mock::sequence s;
    MOCK_EXPECT( visitor.VisitNamedChild ).once().in( s ).with( "child", mock::any ).calls( boost::bind( accept, _2, boost::ref( visitor ) ) );
    MOCK_EXPECT( visitor.VisitInt64 ).once().in( s ).with( 42 );
    MOCK_EXPECT( visitor.VisitNamedChild ).once().in( s ).with( "child 2", mock::any ).calls( boost::bind( accept, _2, boost::ref( visitor ) ) );
    Check( core::MakeModel( "child", 42 )( "child 2", core::MakeModel() ) );
}

BOOST_FIXTURE_TEST_CASE( model_builder_can_build_element_value_model, Fixture)
{
    mock::sequence s;
    MOCK_EXPECT( visitor.VisitElement ).once().in( s ).calls( boost::bind( accept, _1, boost::ref( visitor ) ) );
    MOCK_EXPECT( visitor.VisitInt64 ).once().in( s ).with( 42 );
    Check( core::MakeModel()[ core::MakeModel( 42 ) ] );
}

BOOST_FIXTURE_TEST_CASE( model_builder_can_build_user_data_model, Fixture)
{
    boost::shared_ptr< core::UserData_ABC > data = core::MakeUserData( 42 );
    mock::sequence s;
    MOCK_EXPECT( visitor.VisitUserData ).once().in( s ).with( data );
    Check( core::MakeModel( data ) );
}

BOOST_FIXTURE_TEST_CASE( model_builder_can_build_child_user_data_model, Fixture)
{
    boost::shared_ptr< core::UserData_ABC > data = core::MakeUserData( 42 );
    mock::sequence s;
    MOCK_EXPECT( visitor.VisitNamedChild ).once().in( s ).with( "child", mock::any ).calls( boost::bind( accept, _2, boost::ref( visitor ) ) );
    MOCK_EXPECT( visitor.VisitUserData ).once().in( s ).with( data );
    Check( core::MakeModel( "child", data ) );
}

BOOST_FIXTURE_TEST_CASE( model_builder_can_build_element_user_data_model, Fixture)
{
    boost::shared_ptr< core::UserData_ABC > data = core::MakeUserData( 42 );
    mock::sequence s;
    MOCK_EXPECT( visitor.VisitElement ).once().in( s ).calls( boost::bind( accept, _1, boost::ref( visitor ) ) );
    MOCK_EXPECT( visitor.VisitUserData ).once().in( s ).with( data );
    Check( core::MakeModel()[ data ] );
}
