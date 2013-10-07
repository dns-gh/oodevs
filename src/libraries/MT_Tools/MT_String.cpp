// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "MT_String.h"
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

bool sCaseInsensitiveLess::operator () ( const char* lhs, const char* rhs ) const
{
    return boost::ilexicographical_compare(lhs, rhs);
}

bool sCaseInsensitiveLess::operator () ( const std::string& lhs, const std::string& rhs ) const
{
    return operator () ( lhs.c_str(), rhs.c_str() );
}

bool sCaseInsensitiveLess::operator () ( const char* lhs, const std::string& rhs ) const
{
    return operator () ( lhs, rhs.c_str() );
}

bool sCaseInsensitiveLess::operator () ( const std::string& lhs, const char* rhs ) const
{
    return operator () ( lhs.c_str(), rhs );
}

bool sCaseInsensitiveLess::operator () ( const std::pair< std::string, std::string >& lhs, const std::pair< std::string, std::string >& rhs ) const
{
    return ( operator()( lhs.first, rhs.first ) || !operator()( rhs.first, lhs.first) &&  operator()( lhs.second,  rhs.second ) );
}

