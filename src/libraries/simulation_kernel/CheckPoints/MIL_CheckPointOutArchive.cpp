// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_CheckPointOutArchive.h"
#pragma warning( push , 0 )
#include <boost/archive/impl/basic_binary_oprimitive.ipp>
#pragma warning( pop )
#include <boost/archive/impl/basic_binary_oarchive.ipp>
#include <boost/archive/impl/archive_serializer_map.ipp>

namespace boost
{
namespace archive
{
    template class basic_binary_oprimitive< MIL_CheckPointOutArchive, char, std::char_traits< char> >;
    template class basic_binary_oarchive< MIL_CheckPointOutArchive >;
    template class binary_oarchive_impl< MIL_CheckPointOutArchive, char, std::char_traits< char > >;
    template class detail::archive_serializer_map< MIL_CheckPointOutArchive >;
}
}

MIL_CheckPointOutArchive::MIL_CheckPointOutArchive( std::ostream & os, const boost::shared_ptr< TER_World >& world, unsigned int flags )
    : T_Archive( os, flags )
    , world_( world )
{
    // NOTHING
}

MIL_CheckPointOutArchive::~MIL_CheckPointOutArchive()
{
    // NOTHING
}

const boost::shared_ptr< TER_World >& MIL_CheckPointOutArchive::GetWorld() const
{
    return world_;
}
