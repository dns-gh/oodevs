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
#include <boost/shared_ptr.hpp>

class ObjectTypeResolver_ABC;
class TER_World;

// =============================================================================
/** @class  MIL_CheckPointInArchive
    @brief  MIL_CheckPointInArchive
*/
// Created: RDS 2008-04-02
// =============================================================================
class MIL_CheckPointInArchive :
    public boost::archive::binary_iarchive_impl< MIL_CheckPointInArchive, std::istream::char_type, std::istream::traits_type >,
    public boost::archive::detail::shared_ptr_helper
{
private:
    typedef boost::archive::binary_iarchive_impl< MIL_CheckPointInArchive, std::istream::char_type, std::istream::traits_type > T_Archive;

public:
             MIL_CheckPointInArchive( std::istream& is, const ObjectTypeResolver_ABC& resolver,
                 const boost::shared_ptr< TER_World >& world, unsigned int flags = 0 );
    virtual ~MIL_CheckPointInArchive();

    const ObjectTypeResolver_ABC& GetObjectTypeResolver() const;
    const boost::shared_ptr< TER_World >& GetWorld() const;

    template< typename T >
    void load_override( T& t, int i )
    {
        try
        {
            T_Archive::load_override( t, i );
        }
        catch( tools::Exception& )
        {
            throw;
        }
        catch( std::exception& e )
        {
            throw MASA_EXCEPTION( e.what() );
        }
    }

private:
    const ObjectTypeResolver_ABC& resolver_;
    const boost::shared_ptr< TER_World > world_;
};

BOOST_SERIALIZATION_REGISTER_ARCHIVE( MIL_CheckPointInArchive );

#endif // __MIL_CheckPointInArchive_h_
