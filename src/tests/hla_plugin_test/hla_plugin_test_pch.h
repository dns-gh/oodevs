// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef hla_plugin_test_pch_h
#define hla_plugin_test_pch_h

#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

#define BOOST_TODO BOOST_WARN_MESSAGE( 0, "TODO !" )

#pragma warning( disable: 4505 4244 )
#include <turtle/mock.hpp>

#include <tools/Exception.h>
#include <tools/Path.h>

std::string BOOST_RESOLVE( const std::string& filename );

#include <hla/AttributeIdentifier.h>
#include <hla/ClassIdentifier.h>

namespace hla
{
    inline std::ostream& operator<<( std::ostream& os, const ::hla::AttributeIdentifier& identifier )
    {
        return os << identifier.ToString();
    }
    inline std::ostream& operator<<( std::ostream& os, const hla::ClassIdentifier& identifier )
    {
        return os << identifier.ToString();
    }
}

#include "rpr/EntityType.h"

namespace rpr
{
    inline std::ostream& operator<<( std::ostream& os, const EntityType& type )
    {
        return os << type.str();
    }
}

#include "hla_plugin/UniqueId.h"

namespace plugins
{
namespace hla
{
    template< std::size_t N >
    std::vector< char > MakeUniqueIdGen( const std::string& s )
    {
        std::vector< char > retval( N, 0 );
        std::copy( s.begin(), s.begin() + std::min( N, s.size() ), retval.begin() );
        return retval;
    }
    std::vector< char > MakeUniqueId( const std::string& s );
    NETN_UUID MakeNetnUid( const std::string& s );
}
}

#undef max

#endif // hla_plugin_test_pch_h
