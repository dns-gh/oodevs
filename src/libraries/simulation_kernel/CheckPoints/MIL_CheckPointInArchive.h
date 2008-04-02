// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MIL_CheckPointInArchive_h_
#define __MIL_CheckPointInArchive_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include <boost/archive/binary_iarchive_impl.hpp>
#include <boost/archive/impl/basic_binary_iprimitive.ipp>
#include <boost/archive/impl/basic_binary_iarchive.ipp>
#include <boost/archive/impl/archive_pointer_iserializer.ipp>

// =============================================================================
/** @class  MIL_CheckPointInArchive
    @brief  MIL_CheckPointInArchive
*/
// Created: RDS 2008-04-02
// =============================================================================
class  MIL_CheckPointInArchive : public boost::archive::binary_iarchive_impl<MIL_CheckPointInArchive, std::istream::char_type, std::istream::traits_type>
{
public:

    MIL_CheckPointInArchive(std::istream & is, unsigned int flags = 0) : boost::archive::binary_iarchive_impl<MIL_CheckPointInArchive, std::istream::char_type, std::istream::traits_type>(is, flags) 
        {}
    ~MIL_CheckPointInArchive(){}
}; 

namespace boost {
namespace archive {

template class boost::archive::basic_binary_iprimitive<MIL_CheckPointInArchive, char, std::char_traits<char> >;
template class boost::archive::basic_binary_iarchive<MIL_CheckPointInArchive> ;
template class boost::archive::binary_iarchive_impl<MIL_CheckPointInArchive, char, std::char_traits<char> >;
template class boost::archive::detail::archive_pointer_iserializer<MIL_CheckPointInArchive> ;

} // namespace archive
} // namespace boost

#endif // __MIL_CheckPointInArchive_h_
