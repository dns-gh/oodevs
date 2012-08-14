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

#define BOOST_NO_STD_LOCALE
#define BOOST_LEXICAL_CAST_ASSUME_C_LOCALE

#pragma warning( push, 0 )
#include <boost/serialization/export.hpp>
#include <boost/archive/binary_iarchive_impl.hpp>
#include <boost/archive/shared_ptr_helper.hpp>
#pragma warning( pop )

// =============================================================================
/** @class  MIL_CheckPointInArchive
    @brief  MIL_CheckPointInArchive
*/
// Created: RDS 2008-04-02
// =============================================================================
class  MIL_CheckPointInArchive : public boost::archive::binary_iarchive_impl<MIL_CheckPointInArchive, std::istream::char_type, std::istream::traits_type>
                               , public boost::archive::detail::shared_ptr_helper
{
public:

             MIL_CheckPointInArchive(std::istream & is, unsigned int flags = 0) : boost::archive::binary_iarchive_impl<MIL_CheckPointInArchive, std::istream::char_type, std::istream::traits_type>(is, flags)
             {}
             MIL_CheckPointInArchive(std::streambuf & bsb, unsigned int flags = 0) : boost::archive::binary_iarchive_impl<MIL_CheckPointInArchive, std::istream::char_type, std::istream::traits_type>(bsb, flags)
             {}
    virtual ~MIL_CheckPointInArchive(){}

private:

    //! @name Copy/Assignment
    //@{
    MIL_CheckPointInArchive( const MIL_CheckPointInArchive& );            //!< Copy constructor
    MIL_CheckPointInArchive& operator=( const MIL_CheckPointInArchive& ); //!< Assignment operator
    //@}

};

BOOST_SERIALIZATION_REGISTER_ARCHIVE( MIL_CheckPointInArchive );

#endif // __MIL_CheckPointInArchive_h_