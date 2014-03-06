// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Codec_h_
#define __Codec_h_

#include <xeumeuleu/xml.hpp>
#include <sstream>

namespace tools
{
    //! @name Qt Codec
    //@{
    void SetCodec();
    //@}

    //! @name Xml Codec
    //@{
    template< typename T >
    bool DecodeTime( const std::string& input, T& output )
    {
        if( input.empty() )
            return false;

        static const double epsilon = 1e-6;
        int nValue = 0;
        double fValue = 0.;

        const char timeUnit = *input.rbegin();
        std::string timeValue( input.substr( 0, input.size() - 1 ) );
        std::stringstream strTimeFValue( timeValue ); strTimeFValue.precision( 10 ); strTimeFValue >> fValue;
        std::stringstream strTimeNValue( timeValue ); strTimeNValue >> nValue;
        if( std::abs( static_cast< double >( nValue ) - fValue ) < epsilon )
            fValue = static_cast< T >( nValue );

        switch( timeUnit )
        {
        case 's': output = static_cast< T >( fValue ); break;
        case 'm': output = static_cast< T >( fValue * 60 ); break;
        case 'h': output = static_cast< T >( fValue * 3600 ); break;
        default:
            return false;
        }
        return true;
    }

    template< typename T >
    bool ReadTimeAttribute( xml::xistream& xis, const std::string& name, T& time )
    {
        time = T();
        std::string timeString;
        xis >> xml::optional >> xml::attribute( name, timeString );
        return DecodeTime( timeString, time );
    }
    //@}
}

#endif // __Codec_h_
