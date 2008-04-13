// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __xmlcodecs_h_
#define __xmlcodecs_h_

#include "xeumeuleu/xml.h"
#include <sstream>

namespace tools
{
    template< typename T >
    bool DecodeTime( const std::string& input, T& output )
    {
        if( input.empty() )
            return false;

        const char timeUnit = *input.rbegin();
        std::string       strTimeValue = input.substr( 0, input.size() - 1 );
        std::stringstream strTmp( strTimeValue );
        strTmp >> output;

        switch( timeUnit )
        {
            case 's': break; 
            case 'm': output *= (T)60  ; break;
            case 'h': output *= (T)3600; break;
            default:            
                return false;
        }
        return true;
    }

    template< typename T >
    bool ReadTimeAttribute( xml::xistream& xis, const std::string& name, T& time )
    {
        std::string timeString;
        xis >> xml::optional() >> xml::attribute( name, timeString );
        return DecodeTime( timeString, time );
    }
}

#endif // __xmlcodecs_h_
