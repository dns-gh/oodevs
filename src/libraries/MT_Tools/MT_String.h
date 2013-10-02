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

#include <string>
#include <utility>

//-------------------------------------------------------------------------
/** @name Comparaison de chaînes de caractère */
//-------------------------------------------------------------------------
struct sCaseInsensitiveLess
{
    bool operator () ( const char* lhs, const char* rhs ) const;
    bool operator () ( const std::string& lhs, const std::string& rhs ) const;
    bool operator () ( const char* lhs, const std::string& rhs ) const;
    bool operator () ( const std::string& lhs, const char* rhs ) const;
    bool operator () ( const std::pair< std::string, std::string >& lhs,
            const std::pair< std::string, std::string >& rhs ) const;
};

#endif // __MT_String_h_
