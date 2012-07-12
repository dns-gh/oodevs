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

#pragma warning( disable: 4505 )
#include <turtle/mock.hpp>

namespace mock
{
    namespace detail
    {
        template< typename Expected >
        class close
        {
        public:
            explicit close( const Expected& expected, const Expected& tolerance )
                : expected_ ( expected )
                , tolerance_( tolerance )
            {}
            template< typename Actual >
            bool operator()( const Actual& actual ) const
            {
                return std::abs( expected_ - actual ) < tolerance_;
            }
            friend std::ostream& operator<<( std::ostream& s, const close& c )
            {
                return s << "close( " << c.expected_ << ", " << c.tolerance_ << " )";
            }
        private:
            Expected expected_;
            Expected tolerance_;
        };
    }
    template< typename T >
    mock::constraint< detail::close< T > > close( T expected, T tolerance )
    {
        return detail::close< T >( expected, tolerance );
    }
}

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

#undef max

#endif // hla_plugin_test_pch_h
