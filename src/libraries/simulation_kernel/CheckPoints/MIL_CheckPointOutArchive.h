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
#include <boost/shared_ptr.hpp>
#include <ostream>

class TER_World;

// =============================================================================
/** @class  MIL_CheckPointOutArchive
    @brief  MIL_CheckPointOutArchive
*/
// Created: RDS 2008-04-02
// =============================================================================
class  MIL_CheckPointOutArchive :
    public boost::archive::binary_oarchive_impl< MIL_CheckPointOutArchive, std::ostream::char_type, std::ostream::traits_type >
{
private:
    typedef boost::archive::binary_oarchive_impl< MIL_CheckPointOutArchive, std::ostream::char_type, std::ostream::traits_type > T_Archive;

public:
             MIL_CheckPointOutArchive( std::ostream & os, const boost::shared_ptr< TER_World >& world, unsigned int flags = 0 );
    virtual ~MIL_CheckPointOutArchive();

    // This accessor is probably useless when writing the archive, but it let's
    // us write template code handling load and save at the same time.
    const boost::shared_ptr< TER_World >& GetWorld() const;

    template< typename T >
    void save_override( T& t, int i )
    {
        try
        {
            T_Archive::save_override( t, i );
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
    const boost::shared_ptr< TER_World > world_;
};

BOOST_SERIALIZATION_REGISTER_ARCHIVE( MIL_CheckPointOutArchive );

#endif // __MIL_CheckPointOutArchive_h_
