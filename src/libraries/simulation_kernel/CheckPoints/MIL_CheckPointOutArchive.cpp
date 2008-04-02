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

namespace boost {
namespace archive {
template class boost::archive::basic_binary_oprimitive<MIL_CheckPointOutArchive, char, std::char_traits<char> >;
template class boost::archive::basic_binary_oarchive<MIL_CheckPointOutArchive> ;
template class boost::archive::binary_oarchive_impl<MIL_CheckPointOutArchive, char, std::char_traits<char> >;
template class boost::archive::detail::archive_pointer_oserializer<MIL_CheckPointOutArchive> ;
} // namespace archive
} // namespace boost
