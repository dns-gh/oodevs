// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "protocol_test_pch.h"
#include "protocol/Helpers.h"

#include <boost/lexical_cast.hpp>
#include <unordered_set>

using namespace protocol;
using namespace sword;

namespace
{
    template< typename T >
    std::unordered_set< typename T::value_type > MakeSet()
    {
        std::unordered_set< typename T::value_type > rpy;
        for( size_t i = 0; i < T::size_; ++i )
            rpy.insert( T::data_[i].type );
        return rpy;
    }

    template< typename T, typename U, typename V >
    void CheckMapping( int min, int max, const U& IsValid, const V& GetName )
    {
        const auto set = MakeSet< T >();
        for( int i = min; i <= max; ++i )
        {
            const auto value = static_cast< typename T::value_type >( i );
            const bool valid = IsValid( value );
            BOOST_REQUIRE( valid );
            if( valid )
                BOOST_CHECK_MESSAGE( set.find( value ) != set.end(), "Missing " + GetName( value ) );
        }
    }

    template< typename T, typename U >
    void CheckTypeMapping()
    {
        CheckMapping< T >( U::Type_MIN, U::Type_MAX, U::Type_IsValid, U::Type_Name );
    }
}

BOOST_AUTO_TEST_CASE( check_magic_unit_mappings )
{
    CheckTypeMapping< mapping::MagicUnitAction, UnitMagicAction >();
}

BOOST_AUTO_TEST_CASE( check_magic_knowledge_mappings )
{
    CheckTypeMapping< mapping::MagicKnowledgeAction, KnowledgeMagicAction >();
}

BOOST_AUTO_TEST_CASE( check_magic_object_mappings )
{
    CheckTypeMapping< mapping::MagicObjectAction, ObjectMagicAction >();
}

BOOST_AUTO_TEST_CASE( check_magic_mappings )
{
    CheckTypeMapping< mapping::MagicAction, MagicAction >();
}

BOOST_AUTO_TEST_CASE( check_phaselinetype_mappings )
{
    CheckMapping< mapping::PhaseLineType >(
        PhaseLineOrder::Function_MIN,
        PhaseLineOrder::Function_MAX,
        PhaseLineOrder::Function_IsValid,
        PhaseLineOrder::Function_Name );
}
