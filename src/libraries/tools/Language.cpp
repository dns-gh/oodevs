// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "tools_pch.h"
#include "Language.h"
#include "MT_Tools/MT_Logger.h"
#include <xeumeuleu/xml.h>
#include <QtCore/QString>
#pragma warning( push, 0 )
#pragma warning( disable : 4127 ) // conditional expression is constant
#include <QtCore/qsettings.h>
#pragma warning( pop )

using namespace tools;

std::string Language::current_ = "en";

// -----------------------------------------------------------------------------
// Name: Language constructor
// Created: ABR 2013-10-14
// -----------------------------------------------------------------------------
Language::Language( const Language& other )
    : name_( other.name_ )
    , code_( other.code_ )
    , englishName_( other.englishName_ )
    , supported_( other.supported_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Language constructor
// Created: ABR 2013-07-09
// -----------------------------------------------------------------------------
Language::Language( xml::xistream& xis )
    : name_( xis.attribute( "name", "" ) )
    , code_( xis.attribute( "code", "" ) )
    , englishName_( xis.attribute( "english-name", "" ) )
    , supported_( xis.attribute( "supported", false ) )
{
    if( name_.empty() || code_.empty() )
        throw MASA_EXCEPTION( QString( "Incomplete language '%1' / '%2'" ).arg( name_.c_str() ).arg( code_.c_str() ).toStdString() );
    else if( code_.size() != 2 )
        throw MASA_EXCEPTION( QString( "Invalid language code '%1'" ).arg( code_.c_str() ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: Language destructor
// Created: ABR 2013-07-09
// -----------------------------------------------------------------------------
Language::~Language()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Language::GetName
// Created: ABR 2013-07-09
// -----------------------------------------------------------------------------
const std::string& Language::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: Language::GetEnglishName
// Created: ABR 2013-10-24
// -----------------------------------------------------------------------------
const std::string& Language::GetEnglishName() const
{
    return englishName_;
}

// -----------------------------------------------------------------------------
// Name: Language::GetCode
// Created: ABR 2013-07-09
// -----------------------------------------------------------------------------
const std::string& Language::GetCode() const
{
    return code_;
}

// -----------------------------------------------------------------------------
// Name: Language::IsSupported
// Created: ABR 2013-10-24
// -----------------------------------------------------------------------------
bool Language::IsSupported() const
{
    return supported_;
}

// -----------------------------------------------------------------------------
// Name: Language::Current
// Created: ABR 2013-08-21
// -----------------------------------------------------------------------------
const std::string& Language::Current()
{
    return current_;
}

// -----------------------------------------------------------------------------
// Name: Language::SetCurrent
// Created: ABR 2013-08-21
// -----------------------------------------------------------------------------
void Language::SetCurrent( const std::string& language )
{
    current_ = language;
}

// -----------------------------------------------------------------------------
// Name: Language::InitFromRegistry
// Created: ABR 2013-10-14
// -----------------------------------------------------------------------------
void Language::InitFromRegistry()
{
    QSettings settings( "MASA Group", "SWORD" );
    const QString code = settings.value( "/Common/Language", "en" ).value< QString >();
    if( code.size() == 2 )
        SetCurrent( code.toStdString() );
    else
    {
        MT_LOG_ERROR_MSG( "Invalid language code: " << code.toStdString() << ". Fall back to default language: English." );
        SetCurrent( "en" );
    }
}
