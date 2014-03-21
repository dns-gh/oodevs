// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef xsd_Helpers_h
#define xsd_Helpers_h

#include <string>

namespace core
{
    class Model;
}

namespace tools
{
    class Path;
}

namespace xsd
{
    bool IsBuiltinXsdType( const std::string& type );
    std::string XsdTypeToProtoType( const std::string& type );

    std::string XsdNameToProtoName( const std::string& name );
    std::string ProtoNameToXsdName( const std::string& name );

    std::string XsdEnumToProtoEnum( const std::string& name );
    std::string ProtoEnumToXsdEnum( const std::string& name );
}

#endif // xsd_Helpers_h
