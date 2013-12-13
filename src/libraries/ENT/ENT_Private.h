// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ENT_Private_h_
#define __ENT_Private_h_

#pragma warning( push, 0 )
#include <QApplication>
#pragma warning( pop )

namespace ENT_Tr
{
//-----------------------------------------------------------------------------
// Name: struct converter
// Created: JVT 03-01-08
//-----------------------------------------------------------------------------
template < typename type >
struct Converter
{
    const type        type_;
    const std::string simname_;  // The name used in the scripts
    const std::string appname_;  // The untranslated name used for displaying
    std::string       trname_;   // The translated name used for displaying

    Converter( const char* simname, const char* appname, type t ) : type_( t ), simname_( simname ), appname_( appname ), trname_( "NO TRANSLATION" ) {}
    Converter& operator = ( const Converter& rhs ) { type_ = rhs.type_; simname_ = rhs.simname_; appname_ = rhs.appname_; trname_ = rhs.trname_; return *this; }
};

//-----------------------------------------------------------------------------
// Name: FindInConverter
// Created: JVT 03-01-08
// Last modified: JVT 03-04-29
//-----------------------------------------------------------------------------
template < typename T >
const T& FindInConverter( const Converter<T> conv[], const std::string& str, ENT_Tr::E_Conversion e = ENT_Tr::eToSim )
{
    for( unsigned int i = 0; ; ++i )
    {
        const std::string& sCurrentName = GetName( e, conv[i] );
        if( sCurrentName.empty() || !::_stricmp( sCurrentName.c_str(), str.c_str() ) )
            return conv[i].type_;
    }
    // Unreachable
}

//-----------------------------------------------------------------------------
// Name: InverseFindInConverter
// Created: JVT 03-04-29
//-----------------------------------------------------------------------------
template < typename T >
const std::string& InverseFindInConverter( const Converter<T> conv[], T val , ENT_Tr::E_Conversion e = ENT_Tr::eToTr )
{
    static const std::string strBadName_ = "";

    for( unsigned int i = 0; !conv[i].simname_.empty(); ++i )
    {
        if( conv[i].type_ == val )
            return GetName( e, conv[i] );
    }
    return strBadName_;
}

//-----------------------------------------------------------------------------
// Name: InitTr
// Created: JDY 03-07-24
//-----------------------------------------------------------------------------
template < typename T >
void InitTr( Converter<T> conv[], const char* szContext = "ENT" )
{
    for ( unsigned int i = 0; !conv[i].simname_.empty(); ++i )
        conv[i].trname_ = qApp->translate( szContext, conv[i].appname_.c_str() ).toStdString();
}

template < typename T >
const std::string& GetName( ENT_Tr::E_Conversion e, const Converter<T>& conv )
{
    static const std::string strBadName_;
    switch( e )
    {
        case ENT_Tr::eToSim:
            return conv.simname_;
        case ENT_Tr::eToApp:
            return conv.appname_;
        case ENT_Tr::eToTr:
            return conv.trname_;
    }
    return strBadName_;
}
}

#endif // __ENT_Private_h_
