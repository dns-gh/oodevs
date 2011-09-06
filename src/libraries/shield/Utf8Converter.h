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
    static void ConvertCP1252StringsToUtf8( google::protobuf::Message& message );
    static void ConvertUtf8StringsToCP1252( google::protobuf::Message& message );
    //@}

private:
    //! @name Helpers
    //@{
    static void CP1252ToUtf8( const std::string& ansiString, std::string& utf8String );
    static void Utf8ToCP1252( const std::string& utf8String, std::string& ansiString );
    static unsigned int  CP1252ToUnicode( unsigned char cp1252Character );
    static unsigned char UnicodeToCP1252( unsigned int unicodeCharacter );
    //@}
};

}

#endif // __Utf8_Converter_h_
