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
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#include <tools/Path.h>

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
    for( auto itContext = contexts_.begin(); itContext != contexts_.end(); ++itContext )
        for( auto itTranslation = itContext->second->begin(); itTranslation != itContext->second->end(); )
            if( itTranslation->unique() )
                itTranslation = itContext->second->erase( itTranslation );
            else
                ++itTranslation;

    for( auto itContext = contexts_.begin(); itContext != contexts_.end(); )
        if( itContext->second->empty() || itContext->second.unique() )
            itContext = contexts_.erase( itContext );
        else
            ++itContext;
}

// -----------------------------------------------------------------------------
// Name: XmlTranslations::MergeDuplicateTranslations
// Created: ABR 2013-08-27
// -----------------------------------------------------------------------------
void XmlTranslations::MergeDuplicateTranslations()
{
    CleanTranslations();
    for( auto itContext = contexts_.begin(); itContext != contexts_.end(); ++itContext )
        for( auto lhs = itContext->second->begin(); lhs != itContext->second->end(); ++lhs )
            for( auto rhs = lhs + 1; rhs != itContext->second->end(); )
                if( ( *lhs )->Key() == ( *rhs )->Key() && **lhs == **rhs )
                    rhs = itContext->second->erase( rhs );
                else
                    ++rhs;
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
        if( !contexts_.empty() )
            for( auto itContext = contexts_.begin(); itContext != contexts_.end(); ++itContext )
            {
                if( itContext->second->empty() )
                    continue;
                xos << xml::start( "context" )
                    << xml::start( "name" ) << itContext->first << xml::end;
                for( auto itTranslation = itContext->second->begin(); itTranslation != itContext->second->end(); ++itTranslation )
                {
                    if( ( *itTranslation )->Key().empty() )
                        continue;
                    xos << xml::start( "message" )
                        << xml::start( "source" ) << ( *itTranslation )->Key() << xml::end
                        << xml::start( "translation" )
                        << ( *itTranslation )->Value( languageCode )
                        << ( *itTranslation )->Type( languageCode )
                        << xml::end //! translation
                        << xml::end; //! message
                }
                xos << xml::end; //! context
            }
        xos << xml::end; //! ts
    }
}

// -----------------------------------------------------------------------------
// Name: XmlTranslations::ApplyOnTranslations
// Created: ABR 2013-10-08
// -----------------------------------------------------------------------------
bool XmlTranslations::ApplyOnTranslations( std::function< bool( LocalizedString& ) > functor )
{
    for( auto itContext = contexts_.begin(); itContext != contexts_.end(); ++itContext )
        if( itContext->second->Apply( functor ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Accessors
// -----------------------------------------------------------------------------

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
    const boost::shared_ptr< Context >& context = GetContext( strContext );
    if( context->find( key ) == context->end() )
        context->CreateNew( key );
    return context->at( key );
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
    for( auto itContext = contexts_.begin(); itContext != contexts_.end(); ++itContext )
        for( auto lhs = itContext->second->begin(); lhs != itContext->second->end(); ++lhs )
            for( auto rhs = lhs + 1; rhs != itContext->second->end(); ++rhs )
                if( ( *lhs )->Key() == ( *rhs )->Key() && **lhs != **rhs )
                    return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: XmlTranslations::HasTranslations
// Created: ABR 2013-10-23
// -----------------------------------------------------------------------------
bool XmlTranslations::HasTranslations() const
{
    return hasTranslations_;
}
