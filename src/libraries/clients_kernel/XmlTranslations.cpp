// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "XmlTranslations.h"
#include "Context.h"
#include "Tools.h"
#include "tools/FileWrapper.h"
#include "tools/SchemaWriter.h"
#include "tools/XmlStreamOperators.h"
#include <tools/Path.h>
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: XmlTranslations constructor
// Created: ABR 2013-07-09
// -----------------------------------------------------------------------------
XmlTranslations::XmlTranslations()
    : hasTranslations_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: XmlTranslations destructor
// Created: ABR 2013-07-09
// -----------------------------------------------------------------------------
XmlTranslations::~XmlTranslations()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: XmlTranslations::Purge
// Created: JSR 2013-07-23
// -----------------------------------------------------------------------------
void XmlTranslations::Purge()
{
    contexts_.clear();
}

// -----------------------------------------------------------------------------
// Name: XmlTranslations::LoadTranslation
// Created: ABR 2013-07-10
// -----------------------------------------------------------------------------
void XmlTranslations::LoadTranslationFile( const tools::Path& xmlFile, const tools::Path& localesDirectory, const std::string& languageCode )
{
    if( languageCode.size() != 2 )
        throw MASA_EXCEPTION( "Invalid language code: " + languageCode );
    const tools::Path translationPath = localesDirectory / tools::Path::FromUTF8( languageCode ) / xmlFile.BaseName() + "_" + tools::Path::FromUTF8( languageCode ) + ".ts";
    if( !translationPath.Exists() )
        return;
    tools::Xifstream xis( translationPath );
    LoadTranslationXmlStream( xis, languageCode );
    hasTranslations_ = true;
}

// -----------------------------------------------------------------------------
// Name: XmlTranslations::LoadTranslationXmlStream
// Created: JSR 2013-09-30
// -----------------------------------------------------------------------------
void XmlTranslations::LoadTranslationXmlStream( xml::xistream& xis, const std::string& languageCode )
{
    xis >> xml::start( "TS" )
            >> xml::list( "context", *this, &XmlTranslations::ReadContext, languageCode )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: XmlTranslations::ReadContext
// Created: ABR 2013-07-09
// -----------------------------------------------------------------------------
void XmlTranslations::ReadContext( xml::xistream& xis, const std::string& languageCode )
{
    std::string context = "";
    xis >> xml::start( "name" ) >> context >> xml::end;
    if( !context.empty() )
        xis >> xml::list( "message", *this, &XmlTranslations::ReadMessage, languageCode, context );
}

// -----------------------------------------------------------------------------
// Name: XmlTranslations::ReadMessage
// Created: ABR 2013-07-09
// -----------------------------------------------------------------------------
void XmlTranslations::ReadMessage( xml::xistream& xis, const std::string& languageCode, const std::string& context )
{
    std::string source = "";
    xis >> xml::start( "source" ) >> source >> xml::end;
    std::string translation = "";
    E_TranslationType type = eTranslationType_None;
    xis >> xml::start( "translation" )
        >> xml::optional >> translation
        >> type
        >> xml::end;
    SetTranslation( context, source, languageCode, translation, type );
}

// -----------------------------------------------------------------------------
// Name: XmlTranslations::CleanTranslations
// Created: ABR 2013-08-27
// -----------------------------------------------------------------------------
void XmlTranslations::CleanTranslations()
{
    for( auto it = contexts_.begin(); it != contexts_.end(); ++it )
        it->second->CleanTranslations();
    boost::remove_erase_if( contexts_, [&]( const T_Contexts::value_type& context ){ return context.second->IsEmpty() || context.second.unique();} );
}

// -----------------------------------------------------------------------------
// Name: XmlTranslations::MergeDuplicateTranslations
// Created: ABR 2013-08-27
// -----------------------------------------------------------------------------
void XmlTranslations::MergeDuplicateTranslations()
{
    CleanTranslations();
    for( auto it = contexts_.begin(); it != contexts_.end(); ++it )
        it->second->MergeDuplicateTranslations();
}

// -----------------------------------------------------------------------------
// Name: XmlTranslations::SaveTranslations
// Created: ABR 2013-07-09
// -----------------------------------------------------------------------------
void XmlTranslations::SaveTranslationFiles( const tools::Path& xmlFile, const tools::Path& localesDirectory, const tools::LanguagesVector& languages ) const
{
    if( !hasTranslations_ )
        return;

    for( auto itLanguage = languages.begin(); itLanguage != languages.end(); ++itLanguage )
    {
        const std::string& languageCode = itLanguage->GetCode();
        const tools::Path languageDirectory = localesDirectory / tools::Path::FromUTF8( languageCode );
        if( !languageDirectory.Exists() )
            languageDirectory.CreateDirectories();

        const tools::Path translationPath = languageDirectory / xmlFile.BaseName() + "_" + tools::Path::FromUTF8( languageCode ) + ".ts";
        tools::Xofstream xos( translationPath );
        // $$$$ ABR 2013-07-10: TODO: Insert <!DOCTYPE TS> here when xeumeuleu will handle it
        xos << xml::start( "TS" )
                << xml::attribute( "version", "2.0" )
                << xml::attribute( "language", languageCode );
        for( auto it = contexts_.begin(); it != contexts_.end(); ++it )
        {
            if( it->second->IsEmpty() )
                continue;
            xos     << xml::start( "context" )
                        << xml::content( "name", it->first );
            it->second->Apply( [&]( LocalizedString& translation ) -> bool
            {
                if( translation.Key().empty() )
                    return false;
                xos << xml::start( "message" )
                        << xml::content( "source", translation.Key() )
                        << xml::start( "translation" )
                            << translation.Value( languageCode )
                            << translation.Type( languageCode )
                        << xml::end
                    << xml::end;
                return false;
            } );
            xos     << xml::end;
        }
        xos << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: XmlTranslations::ApplyOnTranslations
// Created: ABR 2013-10-08
// -----------------------------------------------------------------------------
bool XmlTranslations::ApplyOnTranslations( std::function< bool( LocalizedString& ) > functor )
{
    for( auto it = contexts_.begin(); it != contexts_.end(); ++it )
        if( it->second->Apply( functor ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: XmlTranslations::ApplyOnContexts
// Created: SLI 2014-08-27
// -----------------------------------------------------------------------------
bool XmlTranslations::ApplyOnContexts( std::function< bool( Context& ) > functor )
{
    for( auto it = contexts_.begin(); it != contexts_.end(); ++it )
        if( functor( *it->second ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: XmlTranslations::SetTranslation
// Created: ABR 2013-07-10
// -----------------------------------------------------------------------------
void XmlTranslations::SetTranslation( const std::string& context, const std::string& key, const std::string& languageCode, const std::string& translation, E_TranslationType type /* = kernel::eTranslationType_Unfinished */ )
{
    auto it = contexts_.find( context );
    if( it == contexts_.end() )
        contexts_[ context ] = boost::make_shared< Context >();
    boost::shared_ptr< LocalizedString > string = ( *contexts_[ context ] )[ key ];
    string->SetValue( languageCode, translation );
    string->SetType( languageCode, type );
}

// -----------------------------------------------------------------------------
// Name: XmlTranslations::GetTranslation
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
const boost::shared_ptr< LocalizedString >& XmlTranslations::GetTranslation( const std::string& strContext, const std::string& key )
{
    return (*GetContext( strContext ))[ key ];
}

// -----------------------------------------------------------------------------
// Name: XmlTranslations::GetContext
// Created: ABR 2013-08-23
// -----------------------------------------------------------------------------
const boost::shared_ptr< Context >& XmlTranslations::GetContext( const std::string& context )
{
    hasTranslations_ = true;
    if( contexts_.find( context ) == contexts_.end() )
        contexts_[ context ] = boost::make_shared< Context >();
    return contexts_.at( context );
}

// -----------------------------------------------------------------------------
// Name: XmlTranslations::HasDuplicateErrors
// Created: ABR 2013-08-27
// -----------------------------------------------------------------------------
bool XmlTranslations::HasDuplicateErrors() const
{
    for( auto it = contexts_.begin(); it != contexts_.end(); ++it )
        if( it->second->HasDuplicateErrors() )
            return true;
    return false;
}
