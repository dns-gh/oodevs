// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ENT_Tr_ABC_h_
#define __ENT_Tr_ABC_h_

#pragma warning( push )
#pragma warning( disable : 4996 )

#pragma warning( push, 0 )
#include <QtGui/qapplication.h>
#pragma warning( pop )

#include <assert.h>
#include <string>
#include <algorithm>
#include <cctype>

// =============================================================================
/** @class  ENT_Tr_ABC
    @brief  ENT_Tr_ABC
*/
// Created: APE 2005-02-17
// =============================================================================
class ENT_Tr_ABC
{
public:
    enum E_Conversion
    {
        eToSim,
        eToApp,
        eToTr
    };

public:
    //-----------------------------------------------------------------------------
    // Name: struct converter
    // Created: JVT 03-01-08
    //-----------------------------------------------------------------------------
    template < typename type > struct converter
    {
        const type        type_;
        const std::string simname_;     // The name used in the scripts
        const std::string appname_;     // The untranslated name used for displaying
        std::string trname_;            // The translated name used for displaying

        converter( const char* simname, const char* appname, type t ) : type_( t ), simname_( simname ), appname_( appname ), trname_( "NO TRANSLATION" ) {}
        converter& operator = ( const converter& rhs ) { type_ = rhs.type_; simname_ = rhs.simname_; appname_ = rhs.appname_; trname_ = rhs.trname_; return *this; }
    };

    //-----------------------------------------------------------------------------
    // Name: FindInConverter
    // Created: JVT 03-01-08
    // Last modified: JVT 03-04-29
    //-----------------------------------------------------------------------------
    template < typename T > static const T& FindInConverter( const converter<T> conv[], const std::string& str )
    {
        for( unsigned int i = 0; ; ++i )
        {
            const std::string& sCurrentName = conv[i].simname_;
            if( sCurrentName.empty() || !::_stricmp( sCurrentName.c_str(), str.c_str() ) )
                return conv[i].type_;
        }
        // Unreachable
    }

    //-----------------------------------------------------------------------------
    // Name: InverseFindInConverter
    // Created: JVT 03-04-29
    //-----------------------------------------------------------------------------
    template < typename T > static const std::string& InverseFindInConverter( const converter<T> conv[], T val , E_Conversion e = eToTr )
    {
        static std::string strBadName_ = "";

        for( unsigned int i = 0; !conv[i].simname_.empty(); ++i )
        {
            if( conv[i].type_ == val )
            {
                switch( e )
                {
                    case eToSim:
                        return conv[i].simname_;
                    case eToApp:
                        return conv[i].appname_;
                    case eToTr:
                        return conv[i].trname_;
                    default:
                        assert( 0 );
                }
            }
        }
        return strBadName_;
    }

    //-----------------------------------------------------------------------------
    // Name: InitTr
    // Created: JDY 03-07-24
    //-----------------------------------------------------------------------------
    template < typename T > static void InitTr( converter<T> conv[], const char* szContext = "ENT" )
    {
        for ( unsigned int i = 0; !conv[i].simname_.empty(); ++i )
            conv[i].trname_ = qApp->translate( szContext, conv[i].appname_.c_str() ).ascii();
    }
};

#pragma warning( pop )

#endif // __ENT_Tr_ABC_h_
