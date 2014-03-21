// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef xsd_Constraints_h
#define xsd_Constraints_h

#include <map>
#include <string>

namespace xsd
{
    struct Constraint
    {
        Constraint()
        {}
        Constraint( const std::string& type, const std::string& selector, const std::string& field, const std::string& refer = "" )
            : type_    ( type )
            , selector_( selector )
            , field_   ( field )
            , refer_   ( refer )
        {}
        std::string type_;
        std::string selector_;
        std::string field_;
        std::string refer_;
    };
    typedef std::map< std::string, Constraint > T_Constraints;
}

#endif // xsd_Constraints_h
