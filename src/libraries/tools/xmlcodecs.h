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
}

#endif // __xmlcodecs_h_
