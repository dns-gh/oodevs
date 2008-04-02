// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MIL_CheckPointOutArchive_h_
#define __MIL_CheckPointOutArchive_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include <boost/archive/binary_oarchive_impl.hpp>
#include <boost/archive/impl/basic_binary_oprimitive.ipp>
#include <boost/archive/impl/basic_binary_oarchive.ipp>
#include <boost/archive/impl/archive_pointer_oserializer.ipp>

// =============================================================================
/** @class  MIL_CheckPointOutArchive
    @brief  MIL_CheckPointOutArchive
*/
// Created: RDS 2008-04-02
// =============================================================================
class  MIL_CheckPointOutArchive : public boost::archive::binary_oarchive_impl<MIL_CheckPointOutArchive, std::ostream::char_type, std::ostream::traits_type>
{
public:

    MIL_CheckPointOutArchive(std::ostream & os, unsigned int flags = 0) : boost::archive::binary_oarchive_impl<MIL_CheckPointOutArchive, std::ostream::char_type, std::ostream::traits_type>(os, flags) 
        {}
    ~MIL_CheckPointOutArchive(){}
}; 

// #define BOOST_ARCHIVE_CUSTOM_OARCHIVE_TYPES MIL_CheckPointOutArchive
namespace boost {
namespace archive {
template class boost::archive::basic_binary_oprimitive<MIL_CheckPointOutArchive, char, std::char_traits<char> >;
template class boost::archive::basic_binary_oarchive<MIL_CheckPointOutArchive> ;
template class boost::archive::binary_oarchive_impl<MIL_CheckPointOutArchive, char, std::char_traits<char> >;
template class boost::archive::detail::archive_pointer_oserializer<MIL_CheckPointOutArchive> ;
} // namespace archive
} // namespace boost


#endif // __MIL_CheckPointOutArchive_h_
