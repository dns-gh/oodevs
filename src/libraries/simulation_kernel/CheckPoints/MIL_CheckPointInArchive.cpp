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

namespace boost {
namespace archive {

template class basic_binary_iprimitive<MIL_CheckPointInArchive, char, std::char_traits<char> >;
template class basic_binary_iarchive<MIL_CheckPointInArchive> ;
template class binary_iarchive_impl<MIL_CheckPointInArchive, char, std::char_traits<char> >;
template class detail::archive_pointer_iserializer<MIL_CheckPointInArchive> ;

} // namespace archive
} // namespace boost
