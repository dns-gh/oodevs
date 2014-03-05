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

#define BOOST_NO_STD_LOCALE
#define BOOST_LEXICAL_CAST_ASSUME_C_LOCALE

#pragma warning( push, 0 )
#include <boost/serialization/export.hpp>
#include <boost/archive/binary_oarchive_impl.hpp>
#pragma warning( pop )
#include <ostream>

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
             MIL_CheckPointOutArchive(std::streambuf & bsb, unsigned int flags = 0) : boost::archive::binary_oarchive_impl<MIL_CheckPointOutArchive, std::ostream::char_type, std::ostream::traits_type>(bsb, flags)
             {}
    virtual ~MIL_CheckPointOutArchive(){}
};

BOOST_SERIALIZATION_REGISTER_ARCHIVE( MIL_CheckPointOutArchive );

#endif // __MIL_CheckPointOutArchive_h_
