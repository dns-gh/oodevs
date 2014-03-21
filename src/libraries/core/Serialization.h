// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef CORE_SERIALIZATION_H
#define CORE_SERIALIZATION_H

#ifdef __GNUC__
#   pragma GCC system_header
#elif defined _MSC_VER
#   pragma warning( push, 0 )
#   pragma warning( disable: 4512 4702 4181 4503 4996 )
#endif
#include <boost/serialization/variant.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/container/flat_map.hpp>
#include <boost/serialization/extended_type_info.hpp>
#ifdef _MSC_VER
#   pragma warning( pop )
#endif

namespace boost
{
namespace serialization
{
    template< class Archive, class Type, class Key, class Compare, class Allocator >
    inline void save(
        Archive & ar,
        const boost::container::flat_map< Key, Type, Compare, Allocator > &t,
        const unsigned int /*file_version*/ )
    {
        boost::serialization::stl::save_collection<
            Archive,
            boost::container::flat_map< Key, Type, Compare, Allocator >
        >( ar, t );
    }

    template< class Archive, class Type, class Key, class Compare, class Allocator >
    inline void load(
        Archive & ar,
        boost::container::flat_map< Key, Type, Compare, Allocator > &t,
        const unsigned int /*file_version*/ )
    {
        boost::serialization::stl::load_collection<
            Archive,
            boost::container::flat_map< Key, Type, Compare, Allocator >,
            boost::serialization::stl::archive_input_map< Archive, boost::container::flat_map< Key, Type, Compare, Allocator > >,
            boost::serialization::stl::no_reserve_imp< boost::container::flat_map< Key, Type, Compare, Allocator > >
         >( ar, t );
    }

    template< class Archive, class Type, class Key, class Compare, class Allocator >
    inline void serialize(
        Archive & ar,
        boost::container::flat_map< Key, Type, Compare, Allocator > &t,
        const unsigned int file_version
    )
    {
        boost::serialization::split_free( ar, t, file_version );
    }
}
}

#endif // CORE_SERIALIZATION_H
