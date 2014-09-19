// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_CheckPointInArchive.h"
#pragma warning( push , 0 )
#include <boost/archive/impl/basic_binary_iprimitive.ipp>
#pragma warning( pop )
#include <boost/archive/impl/basic_binary_iarchive.ipp>
#include <boost/archive/impl/archive_serializer_map.ipp>

namespace boost
{
namespace archive
{
    template class basic_binary_iprimitive< MIL_CheckPointInArchive, char, std::char_traits< char > >;
    template class basic_binary_iarchive< MIL_CheckPointInArchive >;
    template class binary_iarchive_impl< MIL_CheckPointInArchive, char, std::char_traits< char > >;
    template class detail::archive_serializer_map< MIL_CheckPointInArchive >;
}
}

MIL_CheckPointInArchive::MIL_CheckPointInArchive( std::istream& is, const ObjectTypeResolver_ABC& resolver,
    const boost::shared_ptr< TER_World >& world, unsigned int flags )
    : T_Archive( is, flags )
    , resolver_( resolver )
    , world_( world )
{
    // NOTHING
}

MIL_CheckPointInArchive::~MIL_CheckPointInArchive()
{
    // NOTHING
}

const ObjectTypeResolver_ABC& MIL_CheckPointInArchive::GetObjectTypeResolver() const
{
    return resolver_;
}

const boost::shared_ptr< TER_World >& MIL_CheckPointInArchive::GetWorld() const
{
    return world_;
}
