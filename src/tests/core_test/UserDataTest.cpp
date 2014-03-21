// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "core_test_pch.h"
#include "core/UserData.h"
#include "UserDataStub.h"
#include "TextArchive.h"

namespace
{
    template< typename T >
    core::UserData< T > MakeUserData( T t )
    {
        return core::UserData< T >( t );
    }

    struct UnregisteredType
    {
        template< typename Archive >
        void serialize( Archive&, const unsigned int )
        {}
    };
}

BOOST_AUTO_TEST_CASE( serializing_user_data_from_base_pointer_with_unregistered_type_throws )
{
    std::stringstream s;
    boost::archive::text_oarchive a( s );
    UnregisteredType t;
    core::UserData< UnregisteredType > u( t );
    core::UserData_ABC* b = &u;
    BOOST_CHECK_THROW( a & b, std::exception );
}

SWORD_USER_DATA_EXPORT( core::UserDataStub )

BOOST_AUTO_TEST_CASE( serializing_user_data_from_base_pointer_with_registered_type_is_valid )
{
    std::stringstream s;
    {
        boost::archive::text_oarchive a( s );
        core::UserDataStub t( 42 );
        core::UserData< core::UserDataStub > u( t );
        core::UserData_ABC* b = &u;
        a & b;
    }
    core::UserData_ABC* b = 0;
    boost::archive::text_iarchive a( s );
    a & b;
    BOOST_REQUIRE( b );
    boost::scoped_ptr< core::UserData_ABC > ptr( b );
    core::UserData< core::UserDataStub >* u = dynamic_cast< core::UserData< core::UserDataStub >* >( b );
    BOOST_REQUIRE( u );
    BOOST_REQUIRE( static_cast< core::UserDataStub* >( u->Get() ) );
    BOOST_CHECK_EQUAL( 42, *static_cast< core::UserDataStub* >( u->Get() ) );
}
