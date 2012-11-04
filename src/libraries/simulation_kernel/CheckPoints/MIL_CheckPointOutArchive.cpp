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

namespace boost {
namespace archive {

template class basic_binary_oprimitive<MIL_CheckPointOutArchive, char, std::char_traits<char> >;
template class basic_binary_oarchive<MIL_CheckPointOutArchive>;
template class binary_oarchive_impl<MIL_CheckPointOutArchive, char, std::char_traits<char> >;
template class detail::archive_serializer_map<MIL_CheckPointOutArchive>;

} // namespace archive
} // namespace boost

