// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Utf8_Converter_h_
#define __Utf8_Converter_h_

#include <string>

namespace google
{
namespace protobuf
{
    class Message;
}
}

namespace shield
{
// =============================================================================
/** @class  Utf8Converter
    @brief  Utf8Converter
*/
// Created: RPD 2011-09-05
// =============================================================================
class Utf8Converter
{
public:
    //! @name Operations
    //@{
    static void ConvertAnsiStringsToUtf8( google::protobuf::Message& message );
    static void ConvertUtf8StringsToAnsi( google::protobuf::Message& message );
    //@}

public: //RPD: should be private once we have some higher level unitary tests
    //! @name Helpers
    //@{
    static void AnsiToUtf8( const std::string& ansiString, std::string& utf8String );
    static void Utf8ToAnsi( const std::string& utf8String, std::string& ansiString );
    //@}
};

}

#endif // __Utf8_Converter_h_
