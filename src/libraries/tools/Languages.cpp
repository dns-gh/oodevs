// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "tools_pch.h"
#include "Languages.h"
#include "FileWrapper.h"
#include "MT_Tools/MT_Logger.h"

using namespace tools;

// -----------------------------------------------------------------------------
// Name: Languages constructor
// Created: ABR 2013-09-30
// -----------------------------------------------------------------------------
Languages::Languages( const tools::Path& xmlFile )
{
    tools::Xifstream xis( xmlFile );
    Read( xis );
}

// -----------------------------------------------------------------------------
// Name: Languages constructor
// Created: ABR 2013-10-01
// -----------------------------------------------------------------------------
Languages::Languages( xml::xistream& xis )
{
    Read( xis );
}

// -----------------------------------------------------------------------------
// Name: Languages destructor
// Created: ABR 2013-09-30
// -----------------------------------------------------------------------------
Languages::~Languages()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Languages::Read
// Created: ABR 2013-09-30
// -----------------------------------------------------------------------------
void Languages::Read( xml::xistream& xis )
{
    xis >> xml::start( "languages" )
            >> xml::list( "language", [&]( xml::xistream& x)
            {
                const std::string code = x.attribute( "code", "" );
                for( auto it = languages_.begin(); it != languages_.end(); ++it )
                    if( it->GetCode() == code )
                        throw MASA_EXCEPTION( "Language code already registered: " + code );
                languages_.push_back( tools::Language( x ) );
            })
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: Languages::Find
// Created: ABR 2013-10-14
// -----------------------------------------------------------------------------
const Language* Languages::Find( const std::string& code ) const
{
    for( auto it = languages_.begin(); it != languages_.end(); ++it )
        if( it->GetCode() == code )
            return &*it;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: Languages::Get
// Created: ABR 2013-10-01
// -----------------------------------------------------------------------------
const Language& Languages::Get( const std::string& code ) const
{
    if( const Language* language = Find( code ) )
        return *language;
    throw MASA_EXCEPTION( "Unsupported language: " + code );
}

// -----------------------------------------------------------------------------
// Name: Languages::GetVector
// Created: ABR 2013-10-07
// -----------------------------------------------------------------------------
const LanguagesVector& Languages::GetVector() const
{
    return languages_;
}

// -----------------------------------------------------------------------------
// Name: Languages::EnsureCurrentIsSupported
// Created: ABR 2013-10-14
// -----------------------------------------------------------------------------
bool Languages::EnsureCurrentIsSupported() const
{
    if( !Find( Language::Current() ) )
    {
        MT_LOG_ERROR_MSG( "Unsupported language code: " << Language::Current() << ". Fall back to default language: English." );
        if( !Find( "en" ) )
            MT_LOG_ERROR_MSG( "Default language 'English' not found in Languages.xml." );
        Language::SetCurrent( "en" );
        return false;
    }
    return true;
}
