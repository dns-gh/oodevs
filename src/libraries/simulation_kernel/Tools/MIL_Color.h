// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __MIL_Color_h_
#define __MIL_Color_h_

#include "MIL.h"
#include "protocol/Protocol.h"
#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
    class xostream;
}

class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

// =============================================================================
/** @class  MIL_Color
    @brief  MIL_Color
*/
// Created: LGY 2011-06-27
// =============================================================================
class MIL_Color : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_Color();
    explicit MIL_Color( xml::xistream& xis );
    virtual ~MIL_Color();
    //@}

    //! @name Operations
    //@{
    template< typename T >
    void SendFullState( T& msg ) const;
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive& file, const unsigned int );
    void save( MIL_CheckPointOutArchive& file, const unsigned int ) const;
    void WriteODB( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::string color_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_Color )

namespace
{
    void Convert( const std::string& strColor, sword::RgbColor& message )
    {
        unsigned int color;
        std::stringstream colorStream( strColor );
        colorStream >> std::hex >> color;
        message.set_red( ( color >> 16 ) & 0xff );
        message.set_green( ( color >> 8 ) & 0xff );
        message.set_blue( color & 0xff );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Color::SendFullState
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
template< typename T >
void MIL_Color::SendFullState( T& msg ) const
{
    if( !color_.empty() )
        Convert( color_, *msg().mutable_color() );
}

#endif // __MIL_Color_h_
