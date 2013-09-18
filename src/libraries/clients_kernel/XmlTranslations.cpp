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
#include "Language.h"
#include "TranslationQuery.h"
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
    : currentLanguage_( tools::readLang() )
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
// TranslationQueries operations
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: XmlTranslations::Purge
// Created: JSR 2013-07-23
// -----------------------------------------------------------------------------
void XmlTranslations::Purge()
{
    queries_.clear();
    contexts_.clear();
}

// -----------------------------------------------------------------------------
// Name: XmlTranslations::LoadTranslationQueries
// Created: ABR 2013-07-10
// -----------------------------------------------------------------------------
bool XmlTranslations::LoadTranslationQueries( const tools::Path& xmlFile )
{
    if( !xmlFile.Exists() || xmlFile.IsDirectory() || xmlFile.Extension() != ".xml" )
        return false;
    tools::Xifstream xis( xmlFile );
    xis >> xml::list( boost::bind( &XmlTranslations::ReadTranslationQueries, this, _2, _3, 1 ) );
    return !queries_.empty();
}

// -----------------------------------------------------------------------------
// Name: XmlTranslations::ReadTranslationQueries
// Created: ABR 2013-07-10
// -----------------------------------------------------------------------------
void XmlTranslations::ReadTranslationQueries( const std::string& name, xml::xistream& xis, int depthMax )
{
    if( name == "translation-queries" )
        xis >> xml::list( "translation-query", *this, &XmlTranslations::ReadTranslationQuery );
    else
        if( depthMax != 0 )
            xis >> xml::list( boost::bind( &XmlTranslations::ReadTranslationQueries, this, _2, _3, depthMax - 1 ) );
}

// -----------------------------------------------------------------------------
// Name: XmlTranslations::ReadTranslationQuery
// Created: ABR 2013-07-09
// -----------------------------------------------------------------------------
void XmlTranslations::ReadTranslationQuery( xml::xistream& xis )
{
    queries_.push_back( TranslationQuery( xis ) );
}

// -----------------------------------------------------------------------------
// Name: XmlTranslations::EvaluateTranslationQueries
// Created: ABR 2013-07-10
// -----------------------------------------------------------------------------
void XmlTranslations::EvaluateTranslationQueries( const tools::Path& xmlFile, const T_Languages& languages )
{
    for( auto it = queries_.begin(); it != queries_.end(); ++it )
    {
        QStringList result = it->Evaluate( xmlFile );
        for( auto itKey = result.begin(); itKey != result.end(); ++itKey )
            for( auto itLanguage = languages.begin(); itLanguage != languages.end(); ++itLanguage )
                SetTranslation( it->GetContext(), itKey->toStdString(), itLanguage->GetShortName(), "" );
    }
}

// -----------------------------------------------------------------------------
// Name: XmlTranslations::SaveTranslationQueries
// Created: ABR 2013-07-10
// -----------------------------------------------------------------------------
void XmlTranslations::SaveTranslationQueries( const tools::Path& xmlFile ) const
{
    if( queries_.empty() )
        return;
    tools::Xofstream xos( xmlFile );
    tools::Xifstream xis( xmlFile );
    xis >> xml::list( boost::bind( &XmlTranslations::CopyAndAddTranslationQueries, this, _2, _3, boost::ref( xos ) ) );
}

namespace
{
    void CopyNode( const std::string& name, xml::xistream& xis, xml::xostream& xos )
    {
        xos << xml::content( name, xis );
    }

    tools::Path ExtractSchemaName( xml::xistream& xis )
    {
        return  xis.attribute< tools::Path >( "xsi:noNamespaceSchemaLocation", "" );
    }
}

// -----------------------------------------------------------------------------
// Name: XmlTranslations::ReadRoodNode
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
void XmlTranslations::CopyAndAddTranslationQueries( const std::string& name, xml::xistream& xis, xml::xostream& xos ) const
{
    xos << xml::start( name );
    tools::SchemaWriter writer;
    writer.WritePhysicalSchema( xos, ExtractSchemaName( xis ).BaseName() );
    xis >> xml::list( boost::bind( &CopyNode, _2, _3, boost::ref( xos ) ) );
    SaveTranslationQueries( xos );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: XmlTranslations::SaveTranslationQueries
// Created: ABR 2013-07-10
// -----------------------------------------------------------------------------
void XmlTranslations::SaveTranslationQueries( xml::xostream& xos ) const
{
    xos << xml::start( "translation-queries");
    for( auto it = queries_.begin(); it != queries_.end(); ++it )
        xos << *it;
    xos << xml::end; //! translation-queries
}


// -----------------------------------------------------------------------------
// Context operations
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: XmlTranslations::LoadTranslations
// Created: ABR 2013-07-09
// -----------------------------------------------------------------------------
void XmlTranslations::LoadTranslationFiles( const tools::Path& xmlFile, const tools::Path& localesDirectory, const T_Languages& languages )
{
    for( auto it = languages.begin(); it != languages.end(); ++it )
        LoadTranslationFile( xmlFile, localesDirectory, it->GetShortName() );
}

// -----------------------------------------------------------------------------
// Name: XmlTranslations::LoadTranslation
// Created: ABR 2013-07-10
// -----------------------------------------------------------------------------
void XmlTranslations::LoadTranslationFile( const tools::Path& xmlFile, const tools::Path& localesDirectory, const std::string& language )
{
    if( language.size() != 2 )
        throw MASA_EXCEPTION( "Invalid language " + language );
    const tools::Path translationPath = localesDirectory / tools::Path::FromUTF8( language ) / xmlFile.BaseName() + "_" + tools::Path::FromUTF8( language ) + ".ts";
    if( !translationPath.Exists() )
        return;
    tools::Xifstream xis( translationPath );
    xis >> xml::start( "TS" )
            >> xml::list( "context", *this, &XmlTranslations::ReadContext, language )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: XmlTranslations::ReadContext
// Created: ABR 2013-07-09
// -----------------------------------------------------------------------------
void XmlTranslations::ReadContext( xml::xistream& xis, const std::string& language )
{
    std::string context = "";
    xis >> xml::start( "name" ) >> context >> xml::end;
    if( context.empty() )
        return;
    auto it = contexts_.find( context );
    if( it != contexts_.end() )
        xis >> xml::list( "message", *this, &XmlTranslations::ReadMessage, language, context );
}

// -----------------------------------------------------------------------------
// Name: XmlTranslations::ReadMessage
// Created: ABR 2013-07-09
// -----------------------------------------------------------------------------
void XmlTranslations::ReadMessage( xml::xistream& xis, const std::string& language, const std::string& context )
{
    std::string source = "";
    xis >> xml::start( "source" ) >> source >> xml::end;
    std::string translation = "";
    E_TranslationType type = eTranslationType_None;
    xis >> xml::start( "translation" )
        >> xml::optional >> translation
        >> type
        >> xml::end;
    SetTranslation( context, source, language, translation, type );
}

// -----------------------------------------------------------------------------
// Name: XmlTranslations::CleanTranslations
// Created: ABR 2013-08-27
// -----------------------------------------------------------------------------
void XmlTranslations::CleanTranslations()
{
    for( auto itContext = contexts_.begin(); itContext != contexts_.end(); )
    {
        if( itContext->second->empty() || itContext->second.unique() )
        {
            auto toDelete = itContext;
            ++itContext;
            contexts_.erase( toDelete );
            continue;
        }

        for( auto itTranslation = itContext->second->begin(); itTranslation != itContext->second->end(); )
        {
            if( ( *itTranslation )->Key().empty() || itTranslation->unique()  )
            {
                itTranslation = itContext->second->erase( itTranslation );
                continue;
            }
            ++itTranslation;
        }
        ++itContext;
    }
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
void XmlTranslations::SaveTranslationFiles( const tools::Path& xmlFile, const tools::Path& localesDirectory, const T_Languages& languages ) const
{
    if( contexts_.empty() )
        return;
    for( auto itLanguage = languages.begin(); itLanguage != languages.end(); ++itLanguage )
    {
        const tools::Path languageDirectory = localesDirectory / tools::Path::FromUTF8( itLanguage->GetShortName() );
        if( !languageDirectory.Exists() )
            languageDirectory.CreateDirectories();

        const tools::Path translationPath = languageDirectory / xmlFile.BaseName() + "_" + tools::Path::FromUTF8( itLanguage->GetShortName() ) + ".ts";
        tools::Xofstream xos( translationPath );
        // $$$$ ABR 2013-07-10: TODO: Insert <!DOCTYPE TS> here when xeumeuleu will handle it
        xos << xml::start( "TS" )
            << xml::attribute( "version", "2.0" )
            << xml::attribute( "language", itLanguage->GetShortName() );
        for( auto itContext = contexts_.begin(); itContext != contexts_.end(); ++itContext )
        {
            xos << xml::start( "context" )
                << xml::start( "name" ) << itContext->first << xml::end;
            for( auto itTranslation = itContext->second->begin(); itTranslation != itContext->second->end(); ++itTranslation )
            {
                xos << xml::start( "message" )
                        << xml::start( "source" ) << ( *itTranslation )->Key() << xml::end
                        << xml::start( "translation" )
                            << ( *itTranslation )->Value( itLanguage->GetShortName() )
                            << ( *itTranslation )->Type( itLanguage->GetShortName() )
                        << xml::end //! translation
                    << xml::end; //! message
            }
            xos << xml::end; //! context
        }
        xos << xml::end; //! ts
    }
}


// -----------------------------------------------------------------------------
// Accessors
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: XmlTranslations::Translate
// Created: ABR 2013-07-09
// -----------------------------------------------------------------------------
const std::string XmlTranslations::Translate( const std::string& key, const std::string& context /*= ""*/, const std::string& language /*= ""*/ ) const
{
    if( currentLanguage_.empty() && language.empty() )
        return "";

    const std::string lang = language.empty() ? currentLanguage_ : language;
    if( context.empty() )
    {
        for( auto it = contexts_.begin(); it != contexts_.end(); ++it )
            if( !it->first.empty() )
            {
                const std::string result = Translate( key, it->first, lang );
                if( !result.empty() )
                    return result;
            }
        return "";
    }

    const std::string& translation = contexts_.at( context )->at( key )->Value( lang );
    return ( !translation.empty() ) ? translation : key;
}

// -----------------------------------------------------------------------------
// Name: XmlTranslations::SetTranslation
// Created: ABR 2013-07-10
// -----------------------------------------------------------------------------
void XmlTranslations::SetTranslation( const std::string& context, const std::string& key, const std::string& language, const std::string& translation, E_TranslationType type /* = kernel::eTranslationType_Unfinished */ )
{
    auto it = contexts_.find( context );
    if( it == contexts_.end() )
        contexts_[ context ] = boost::make_shared< Context >();
    boost::shared_ptr< LocalizedString > string = ( *contexts_[ context ] )[ key ];
    string->SetValue( language, translation );
    string->SetType( language, type );
}

// -----------------------------------------------------------------------------
// Name: XmlTranslations::GetTranslation
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
const boost::shared_ptr< LocalizedString >& XmlTranslations::GetTranslation( const std::string& context, const std::string& key ) const
{
    return contexts_.at( context )->at( key );
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< Context > XmlTranslations::GetContext
// Created: ABR 2013-08-23
// -----------------------------------------------------------------------------
const boost::shared_ptr< Context >& XmlTranslations::GetContext( const std::string& context )
{
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
