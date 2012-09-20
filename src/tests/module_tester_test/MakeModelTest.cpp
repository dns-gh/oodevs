// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "module_tester_test_pch.h"
#include "module_tester/MakeModel.h"
#include <core/Model.h>

using namespace sword::test;

namespace
{
    struct Fixture
    {
        MOCK_FUNCTOR( f, void( SWORD_Model* ) );
        core::Model model;
    };
}

BOOST_FIXTURE_TEST_CASE( make_model_can_build_an_empty_model_constraint, Fixture )
{
    MOCK_EXPECT( f ).once().with( MakeModel() );
    f( core::Convert( &model ) );
}

BOOST_FIXTURE_TEST_CASE( make_model_can_build_data_model_constraint, Fixture )
{
    MOCK_EXPECT( f ).once().with( MakeModel( 42 ) );
    model = 42;
    f( core::Convert( &model ) );
}

namespace
{
    bool CustomContraint( const core::Model& m )
    {
        return m == 42;
    }
}

BOOST_FIXTURE_TEST_CASE( make_model_can_build_custom_constraint_model_constraint, Fixture )
{
    MOCK_EXPECT( f ).once().with( MakeModel( &CustomContraint ) );
    model = 42;
    f( core::Convert( &model ) );
}

BOOST_FIXTURE_TEST_CASE( make_model_can_build_any_constraint_model_constraint, Fixture )
{
    MOCK_EXPECT( f ).once().with( MakeModel( mock::any ) );
    model = 42;
    f( core::Convert( &model ) );
}

BOOST_FIXTURE_TEST_CASE( make_user_data_can_build_a_user_data_constraint, Fixture )
{
    int data = 42;
    MOCK_EXPECT( f ).once().with( MakeModel( MakeUserData( &data ) ) );
    model.SetUserData( &data );
    f( core::Convert( &model ) );
}
