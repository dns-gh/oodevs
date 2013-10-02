// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __MT_String_h_
#define __MT_String_h_

#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )
#undef Yield

//-------------------------------------------------------------------------
/** @name Comparaison de chaînes de caractère */
//-------------------------------------------------------------------------
struct sCaseInsensitiveLess
{
    bool operator () ( const char* lhs, const char* rhs ) const
    {
        return boost::ilexicographical_compare(lhs, rhs);
    }

    bool operator () ( const std::string& lhs, const std::string& rhs ) const
    {
        return operator () ( lhs.c_str(), rhs.c_str() );
    }

    bool operator () ( const char* lhs, const std::string& rhs ) const
    {
        return operator () ( lhs, rhs.c_str() );
    }

    bool operator () ( const std::string& lhs, const char* rhs ) const
    {
        return operator () ( lhs.c_str(), rhs );
    }

    bool operator () ( const std::pair< std::string, std::string >& lhs, const std::pair< std::string, std::string >& rhs ) const
    {
        return ( operator()( lhs.first, rhs.first ) || !operator()( rhs.first, lhs.first) &&  operator()( lhs.second,  rhs.second ) );
    }
};

#endif // __MT_String_h_
