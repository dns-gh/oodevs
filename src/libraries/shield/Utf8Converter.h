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

#include <vector>
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>

namespace shield
{

using namespace google::protobuf;
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
    template< typename T >
    static void ConvertAnsiStringsToUtf8( T& message );
    template< typename T >
    static void ConvertUtf8StringsToAnsi( T& message );
    //@}

public: //RPD: should be private once we have some higher level unitary tests
    //! @name Helpers
    //@{
    static void AnsiToUtf8( const std::string &ansiString, std::string& utf8String );
    static void Utf8ToAnsi( const std::string& utf8String, std::string &ansiString );
    //@}
};

// -----------------------------------------------------------------------------
// Name: Utf8Converter::ConvertAnsiStringsToUtf8
// Created: RPD 2011-09-05
// -----------------------------------------------------------------------------
template< typename T >
void Utf8Converter::ConvertAnsiStringsToUtf8( T& message )
{
    const Message::Reflection* reflection = message.GetReflection();
    vector< const FieldDescriptor * > fields;
    reflection->ListFields( message, &fields );
    std::string utf8converted, ansiString;
    for ( int i = 0; i < fields.size(); ++i)
    {
        const FieldDescriptor* descriptor = fields[i];
        switch ( descriptor->type() )
        {
            case FieldDescriptor::TYPE_STRING:
            {
                if ( descriptor->is_repeated() )
                {
                    int size = reflection->FieldSize( message, descriptor );
                    for ( int index = 0; index < size; ++index )
                    {
                        ansiString = reflection->GetRepeatedString( message, descriptor, index );
                        AnsiToUtf8( ansiString, utf8converted );
                        reflection->SetRepeatedString( &message, descriptor, index, utf8converted );
                    }
                }
                else if ( reflection->HasField( message, descriptor ) )
                {
                    ansiString = reflection->GetString( message, descriptor );
                    AnsiToUtf8( ansiString, utf8converted );
                    reflection->SetString( &message, descriptor, utf8converted );
                }
                break;
            }

            case FieldDescriptor::TYPE_MESSAGE:
            {
                if ( descriptor->is_repeated() )
                    for ( int index = 0; index < reflection->FieldSize( message, descriptor ); ++index )
                        ConvertAnsiStringsToUtf8( *reflection->MutableRepeatedMessage( &message, descriptor, index ) );
                else if ( reflection->HasField( message, descriptor ) )
                    ConvertAnsiStringsToUtf8( *reflection->MutableMessage( &message, descriptor ) );
                break;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Utf8Converter::ConvertAnsiStringsToUtf8
// Created: RPD 2011-09-05
// -----------------------------------------------------------------------------
template< typename T >
void Utf8Converter::ConvertUtf8StringsToAnsi( T& message )
{
    const Message::Reflection* reflection = message.GetReflection();
    vector< const FieldDescriptor * > fields;
    reflection->ListFields( message, &fields );
    std::string utf8string, ansiConverted;
    for ( int i = 0; i < fields.size(); ++i)
    {
        const FieldDescriptor* descriptor = fields[i];
        switch ( descriptor->type() )
        {
            case FieldDescriptor::TYPE_STRING:
            {
                if ( descriptor->is_repeated() )
                {
                    int size = reflection->FieldSize( message, descriptor );
                    for ( int index = 0; index < size; ++index )
                    {
                        utf8string = reflection->GetRepeatedString( message, descriptor, index );
                        Utf8ToAnsi( utf8string, ansiConverted );
                        reflection->SetRepeatedString( &message, descriptor, index, ansiConverted );
                    }
                }
                else if ( reflection->HasField( message, descriptor ) )
                {
                    utf8string = reflection->GetString( message, descriptor );
                    Utf8ToAnsi( utf8string, ansiConverted );
                    reflection->SetString( &message, descriptor, ansiConverted );
                }
                break;
            }

            case FieldDescriptor::TYPE_MESSAGE:
            {
                if ( reflection->HasField( message, descriptor ) )
                {
                    if ( descriptor->is_repeated() )
                        for ( int index = 0; index < reflection->FieldSize( message, descriptor ); ++index )
                            ConvertUtf8StringsToAnsi( *reflection->MutableRepeatedMessage( &message, descriptor, index ) );
                    else
                        ConvertUtf8StringsToAnsi( *reflection->MutableMessage( &message, descriptor ) );
                }
                break;
            }
        }
    }
}

}

#endif // __Utf8_Converter_h_
