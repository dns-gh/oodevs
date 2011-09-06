// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Utf8Converter.h"
#pragma warning( push, 0 )
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#pragma warning( pop )

using namespace shield;
using namespace google::protobuf;

// -----------------------------------------------------------------------------
// Name: Utf8Converter::ConvertCP1252StringsToUtf8
// Created: RPD 2011-09-05
// -----------------------------------------------------------------------------
void Utf8Converter::ConvertCP1252StringsToUtf8( Message& message )
{
    const Message::Reflection* reflection = message.GetReflection();
    std::vector< const FieldDescriptor * > fields;
    reflection->ListFields( message, &fields );
    std::string utf8converted, cp1252String;
    for( std::size_t i = 0; i < fields.size(); ++i )
    {
        const FieldDescriptor* descriptor = fields[i];
        switch( descriptor->type() )
        {
            case FieldDescriptor::TYPE_STRING:
            {
                if( descriptor->is_repeated() )
                {
                    int size = reflection->FieldSize( message, descriptor );
                    for( int index = 0; index < size; ++index )
                    {
                        cp1252String = reflection->GetRepeatedString( message, descriptor, index );
                        CP1252ToUtf8( cp1252String, utf8converted );
                        reflection->SetRepeatedString( &message, descriptor, index, utf8converted );
                    }
                }
                else if( reflection->HasField( message, descriptor ) )
                {
                    cp1252String = reflection->GetString( message, descriptor );
                    CP1252ToUtf8( cp1252String, utf8converted );
                    reflection->SetString( &message, descriptor, utf8converted );
                }
                break;
            }
            case FieldDescriptor::TYPE_MESSAGE:
            {
                if( descriptor->is_repeated() )
                    for( int index = 0; index < reflection->FieldSize( message, descriptor ); ++index )
                        ConvertCP1252StringsToUtf8( *reflection->MutableRepeatedMessage( &message, descriptor, index ) );
                else if( reflection->HasField( message, descriptor ) )
                    ConvertCP1252StringsToUtf8( *reflection->MutableMessage( &message, descriptor ) );
                break;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Utf8Converter::ConvertCP1252StringsToUtf8
// Created: RPD 2011-09-05
// -----------------------------------------------------------------------------
void Utf8Converter::ConvertUtf8StringsToCP1252( Message& message )
{
    const Message::Reflection* reflection = message.GetReflection();
    std::vector< const FieldDescriptor * > fields;
    reflection->ListFields( message, &fields );
    std::string utf8string, cp1252Converted;
    for( std::size_t i = 0; i < fields.size(); ++i )
    {
        const FieldDescriptor* descriptor = fields[i];
        switch( descriptor->type() )
        {
            case FieldDescriptor::TYPE_STRING:
            {
                if( descriptor->is_repeated() )
                {
                    int size = reflection->FieldSize( message, descriptor );
                    for( int index = 0; index < size; ++index )
                    {
                        utf8string = reflection->GetRepeatedString( message, descriptor, index );
                        Utf8ToCP1252( utf8string, cp1252Converted );
                        reflection->SetRepeatedString( &message, descriptor, index, cp1252Converted );
                    }
                }
                else if( reflection->HasField( message, descriptor ) )
                {
                    utf8string = reflection->GetString( message, descriptor );
                    Utf8ToCP1252( utf8string, cp1252Converted );
                    reflection->SetString( &message, descriptor, cp1252Converted );
                }
                break;
            }
            case FieldDescriptor::TYPE_MESSAGE:
            {
                if( descriptor->is_repeated() )
                {
                    for( int index = 0; index < reflection->FieldSize( message, descriptor ); ++index )
                        ConvertUtf8StringsToCP1252( *reflection->MutableRepeatedMessage( &message, descriptor, index ) );
                }
                else if( reflection->HasField( message, descriptor ) )
                {
                    ConvertUtf8StringsToCP1252( *reflection->MutableMessage( &message, descriptor ) );
                }
                break;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Utf8Converter::CP1252ToUtf8
// Created: RPD 2011-09-05
// -----------------------------------------------------------------------------
void Utf8Converter::CP1252ToUtf8( const std::string &cp1252String, std::string& utf8String )
{
    const char* cp1252Content = cp1252String.c_str();
    unsigned char* result = new unsigned char[1 + cp1252String.size() * 4];
    unsigned int n ( 0 );
    unsigned int  i ( cp1252String.size() );
    const unsigned char* incoming = reinterpret_cast< const unsigned char* >( cp1252Content );
    while ( i > 0 )
    {
        unsigned int character = CP1252ToUnicode( *incoming );
        if( character < 0x80 )
        {
            result[n++] = static_cast< unsigned char >( character );
        }
        else if( character < 0x800 )
        {
            result[n++] = static_cast< unsigned char >( 0xC0 | ( character >> 6 ) );
            result[n++] = static_cast< unsigned char >( 0x80 | ( character & 0x3F ) );
        }
        else if( character < 0x10000 )
        {
            result[n++] = static_cast< unsigned char >( 0xE0 | ( character >> 12 ) );
            result[n++] = static_cast< unsigned char >( 0x80 | ( ( character >> 6 ) & 0x3F ) );
            result[n++] = static_cast< unsigned char >( 0x80 | ( character & 0x3F ) );
        }
        /*
        else if( character < 0x200000 ) //will not happen in that case, but just for the sake of utf8 theory...
        {
            result[n++] = static_cast< unsigned char >( 0xF0 | ( character >> 18 ) );
            result[n++] = static_cast< unsigned char >( 0x80 | ( ( character >> 12 ) & 0x3F ) );
            result[n++] = static_cast< unsigned char >( 0x80 | ( ( character >> 6 ) & 0x3F ) );
            result[n++] = static_cast< unsigned char >( 0x80 | ( character & 0x3F ) );
        }
        */
        ++incoming;
        --i;
    }
    result[n] = 0;
    utf8String = reinterpret_cast< char* >( result );
    delete result;
}

// -----------------------------------------------------------------------------
// Name: Utf8Converter::Utf8ToCP1252
// Created: RPD 2011-09-05
// -----------------------------------------------------------------------------
void Utf8Converter::Utf8ToCP1252( const std::string& utf8String, std::string &cp1252String )
{
    const char* utf8content = utf8String.c_str();
    unsigned char* result = new unsigned char[1 + utf8String.size()];
    unsigned int n ( 0 );
    unsigned int i ( utf8String.size() );
    const unsigned char* incoming = reinterpret_cast< const unsigned char* >( utf8content );
    while ( i > 0 )
    {
        unsigned int character = *incoming;
        if( character >= 0xF0 )
        {
            if( i >= 4 )   // four-byte encoded, 21 bits
            {
                character = ( ( incoming[0] & 0x07 ) << 18 ) | ( ( incoming[1] & 0x3F ) << 12 ) | ( ( incoming[2] & 0x3F ) << 6 ) | ( incoming[3] & 0x3F );
            }
            else
            {
                character = '?';    //cp1252 conversion impossible
            }
            incoming += 4;
            i -= 4;
        }
        else if( character >= 0xE0 )
        {
            if( i >= 3 )   // three-byte encoded, 16 bits
            {
                character = ( ( incoming[0] & 0x0F ) << 12 ) | ( ( incoming[1] & 0x3F ) << 6 ) | ( incoming[2] & 0x3F );
            }
            else
            {
                character = '?';
            }
            incoming += 3;
            i -= 3;
        }
        else if( character >= 0xC0 )
        {
            if( i >= 2 ) // two-byte encoded, 11 bits
            {
                character = ( ( incoming[0] & 0x1F ) << 6 ) | ( incoming[1] & 0x3F );
            }
            else
            {
                character = '?';
            }
            incoming += 2;
            i -= 2;
        }
        else
        {
            ++incoming;
            --i;
        }
        character = UnicodeToCP1252( character );
        result[n++] = static_cast< unsigned char >( ( character > 0xFF ) ? '?' : character );
    }
    result[n] = 0;
    cp1252String = reinterpret_cast< char* >( result );
    delete result;
}

// -----------------------------------------------------------------------------
// Name: Utf8Converter::CP1252ToUnicode
// Created: RPD 2011-09-06
// -----------------------------------------------------------------------------
unsigned int Utf8Converter::CP1252ToUnicode( unsigned char cp1252Character )
{
    if ( cp1252Character < 0x80 || cp1252Character > 0x9F)
        return static_cast< unsigned int >( cp1252Character );
    unsigned int unicodeCharacter ( '?' );
    switch ( cp1252Character ) //probably quicker than anything else assembly-wise!
    {
        case 128: unicodeCharacter = 0x20AC; break; // euro sign
        case 130: unicodeCharacter = 0x201A; break; // closing simple curved quote
        case 131: unicodeCharacter = 0x0192; break; //
        case 132: unicodeCharacter = 0x201E; break; // closing double curved quote
        case 133: unicodeCharacter = 0x2026; break; //
        case 134: unicodeCharacter = 0x2020; break; //
        case 135: unicodeCharacter = 0x2021; break; //
        case 136: unicodeCharacter = 0x02C6; break; //
        case 137: unicodeCharacter = 0x2030; break; //
        case 138: unicodeCharacter = 0x0160; break; //
        case 139: unicodeCharacter = 0x2039; break; //
        case 140: unicodeCharacter = 0x0152; break; // OE
        case 142: unicodeCharacter = 0x017D; break; //
        case 145: unicodeCharacter = 0x2018; break; // opening simple curved quote
        case 146: unicodeCharacter = 0x2019; break; // closing simple curved quote
        case 147: unicodeCharacter = 0x201C; break; // opening double curved quote
        case 148: unicodeCharacter = 0x201D; break; // closing double curved quote
        case 149: unicodeCharacter = 0x2022; break; //
        case 150: unicodeCharacter = 0x2013; break; //
        case 151: unicodeCharacter = 0x2014; break; //
        case 152: unicodeCharacter = 0x02DC; break; //
        case 153: unicodeCharacter = 0x2122; break; //
        case 154: unicodeCharacter = 0x0161; break; //
        case 155: unicodeCharacter = 0x203A; break; //
        case 156: unicodeCharacter = 0x0153; break; // oe
        case 158: unicodeCharacter = 0x017E; break; //
        case 159: unicodeCharacter = 0x0178; break; //
    }
    return unicodeCharacter;
}

// -----------------------------------------------------------------------------
// Name: Utf8Converter::UnicodeToCP1252
// Created: RPD 2011-09-06
// -----------------------------------------------------------------------------
unsigned char Utf8Converter::UnicodeToCP1252( unsigned int unicodeCharacter )
{
    if ( unicodeCharacter < 0xFF && ( unicodeCharacter > 0x9F || unicodeCharacter < 0x80 ) )
        return static_cast< unsigned char >( unicodeCharacter );
    unsigned char cp1252Character ( '?' );
    switch ( unicodeCharacter ) //probably quicker than anything else assembly-wise!
    {
        case 0x20AC: cp1252Character = 128; break; // euro sign
        case 0x201A: cp1252Character = 130; break; // closing simple curved quote
        case 0x0192: cp1252Character = 131; break; //
        case 0x201E: cp1252Character = 132; break; // closing double curved quote
        case 0x2026: cp1252Character = 133; break; //
        case 0x2020: cp1252Character = 134; break; //
        case 0x2021: cp1252Character = 135; break; //
        case 0x02C6: cp1252Character = 136; break; //
        case 0x2030: cp1252Character = 137; break; //
        case 0x0160: cp1252Character = 138; break; //
        case 0x2039: cp1252Character = 139; break; //
        case 0x0152: cp1252Character = 140; break; // OE
        case 0x017D: cp1252Character = 142; break; //
        case 0x2018: cp1252Character = 145; break; // opening simple curved quote
        case 0x2019: cp1252Character = 146; break; // closing simple curved quote
        case 0x201C: cp1252Character = 147; break; // opening double curved quote
        case 0x201D: cp1252Character = 148; break; // closing double curved quote
        case 0x2022: cp1252Character = 149; break; //
        case 0x2013: cp1252Character = 150; break; //
        case 0x2014: cp1252Character = 151; break; //
        case 0x02DC: cp1252Character = 152; break; //
        case 0x2122: cp1252Character = 153; break; //
        case 0x0161: cp1252Character = 154; break; //
        case 0x203A: cp1252Character = 155; break; //
        case 0x0153: cp1252Character = 156; break; // oe
        case 0x017E: cp1252Character = 158; break; //
        case 0x0178: cp1252Character = 159; break; //
    }
    return cp1252Character;
}
