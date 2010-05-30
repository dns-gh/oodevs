// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Exercise.h"
#include "clients_kernel/Controller.h"
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: Exercise constructor
// Created: SBO 2010-03-08
// -----------------------------------------------------------------------------
Exercise::Exercise( kernel::Controller& controller )
    : controller_( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Exercise destructor
// Created: SBO 2010-03-08
// -----------------------------------------------------------------------------
Exercise::~Exercise()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Exercise::Load
// Created: SBO 2010-03-08
// -----------------------------------------------------------------------------
void Exercise::Load( xml::xistream& xis )
{
    std::string name, version;
    xis >> xml::start( "exercise" )
        >> xml::attribute( "generator-version", version )
        >> xml::optional() >> xml::start( "meta" )
                >> xml::optional() >> xml::content( "name", name )
                >> xml::optional() >> xml::start( "briefing" )
                    >> xml::list( "text", *this, &Exercise::ReadBriefing )
                >> xml::end()
                >> xml::optional() >> xml::start( "resources" )
                    >> xml::list( "resource", *this, &Exercise::ReadResource )
                >> xml::end()
            >> xml::end();
    xis >> xml::end();
    name_ = name.c_str();
    generatorVersion_ = version.c_str();
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: Exercise::ReadBriefing
// Created: SBO 2010-03-11
// -----------------------------------------------------------------------------
void Exercise::ReadBriefing( xml::xistream& xis )
{
    std::string lang, text;
    xis >> xml::attribute( "lang", lang )
        >> text;
    briefings_[lang.c_str()] = text.c_str();
}

// -----------------------------------------------------------------------------
// Name: Exercise::ReadResource
// Created: SBO 2010-03-11
// -----------------------------------------------------------------------------
void Exercise::ReadResource( xml::xistream& xis )
{
    std::string name, file;
    xis >> xml::attribute( "name", name )
        >> xml::attribute( "file", file );
    resources_[name.c_str()] = file.c_str();
}
// -----------------------------------------------------------------------------
// Name: Exercise::Purge
// Created: SBO 2010-03-08
// -----------------------------------------------------------------------------
void Exercise::Purge()
{
    name_ = "";
    briefings_.clear();
    resources_.clear();
    controller_.Update( *this );
}

namespace
{
    void CopyNode( const std::string& name, xml::xistream& xis, xml::xostream& xos )
    {
        if( name != "meta" )
            xos << xml::content( name, xis );
    }
    void CopyFromFile( const std::string& file, xml::xostream& xos )
    {
        xml::xifstream xis( file );
        xis >> xml::start( "exercise" )
                >> xml::list( boost::bind( &::CopyNode, _1, _2, boost::ref( xos ) ) )
            >> xml::end();
    }
}

// -----------------------------------------------------------------------------
// Name: Exercise::Serialize
// Created: SBO 2010-03-08
// -----------------------------------------------------------------------------
void Exercise::Serialize( const std::string& file ) const
{
    xml::xofstream xos( file, xml::encoding( "UTF-8" ) );
    xos << xml::start( "exercise" )
        << xml::attribute( "generator-version", generatorVersion_.ascii() )
            << xml::start( "meta" );
    if( ! name_.isEmpty() )
        xos << xml::start( "name" ) << xml::cdata( name_.ascii() ) << xml::end();
    SerializeBriefings( xos );
    SerializeResources( xos );
    xos     << xml::end();
    CopyFromFile( file, xos );
    xos << xml::end();
}

// -----------------------------------------------------------------------------
// Name: Exercise::SerializeBriefings
// Created: SBO 2010-03-11
// -----------------------------------------------------------------------------
void Exercise::SerializeBriefings( xml::xostream& xos ) const
{
    xos << xml::start( "briefing" );
    for( T_Resources::const_iterator it = briefings_.begin(); it != briefings_.end(); ++it )
        if( !it->second.isEmpty() )
            xos << xml::start( "text" )
                    << xml::attribute( "lang", it->first.ascii() )
                    << xml::cdata( it->second.ascii() )
                << xml::end();
    xos << xml::end();
}

// -----------------------------------------------------------------------------
// Name: Exercise::SerializeResources
// Created: SBO 2010-03-11
// -----------------------------------------------------------------------------
void Exercise::SerializeResources( xml::xostream& xos ) const
{
    xos << xml::start( "resources" );
    for( T_Resources::const_iterator it = resources_.begin(); it != resources_.end(); ++it )
        xos << xml::start( "resource" )
                << xml::attribute( "name", it->first.ascii() )
                << xml::attribute( "file", it->second.ascii() )
            << xml::end();
    xos << xml::end();
}

// -----------------------------------------------------------------------------
// Name: Exercise::GetName
// Created: SBO 2010-03-11
// -----------------------------------------------------------------------------
QString Exercise::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: Exercise::SetName
// Created: SBO 2010-03-11
// -----------------------------------------------------------------------------
void Exercise::SetName( const QString& name )
{
    name_ = name;
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Exercise::SetBriefing
// Created: SBO 2010-03-11
// -----------------------------------------------------------------------------
void Exercise::SetBriefing( const QString& lang, const QString& text )
{
    briefings_[lang] = text;
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Exercise::AddResource
// Created: SBO 2010-03-11
// -----------------------------------------------------------------------------
void Exercise::AddResource( const QString& name, const QString& file )
{
    resources_[name] = file;
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Exercise::ClearResources
// Created: SBO 2010-03-11
// -----------------------------------------------------------------------------
void Exercise::ClearResources()
{
    resources_.clear();
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Exercise::Accept
// Created: SBO 2010-03-11
// -----------------------------------------------------------------------------
void Exercise::Accept( ExerciseVisitor_ABC& visitor ) const
{
    for( T_Resources::const_iterator it = briefings_.begin(); it != briefings_.end(); ++it )
        visitor.VisitBriefing( it->first, it->second );
    for( T_Resources::const_iterator it = resources_.begin(); it != resources_.end(); ++it )
        visitor.VisitResource( it->first, it->second );
}
