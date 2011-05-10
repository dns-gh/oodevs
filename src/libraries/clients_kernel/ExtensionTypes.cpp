// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "ExtensionTypes.h"
#include "AttributeType.h"
#include "DictionaryType.h"
#include "ExtensionType.h"
#include "tools/ExerciseConfig.h"
#include "tools/Loader_ABC.h"
#include <xeumeuleu/xml.hpp>
#pragma warning( push, 0 )
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#pragma warning( pop )
#include <boost/bind.hpp>

namespace bfs = boost::filesystem;

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ExtensionTypes constructor
// Created: JSR 2010-10-01
// -----------------------------------------------------------------------------
ExtensionTypes::ExtensionTypes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExtensionTypes destructor
// Created: JSR 2010-10-01
// -----------------------------------------------------------------------------
ExtensionTypes::~ExtensionTypes()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: ExtensionTypes::Load
// Created: JSR 2010-10-01
// -----------------------------------------------------------------------------
void ExtensionTypes::Load( const tools::ExerciseConfig& config, const std::string& file )
{
    if( ! bfs::exists( bfs::path( file, bfs::native ) ) )
        return;
    config.GetLoader().LoadFile( file, boost::bind( &ExtensionTypes::ReadExtensions, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: ExtensionTypes::ReadExtensions
// Created: MCO 2010-11-21
// -----------------------------------------------------------------------------
void ExtensionTypes::ReadExtensions( xml::xistream& xis )
{
    xis >> xml::start( "extensions" )
            >> xml::list( *this, &ExtensionTypes::ReadElement );
}

// -----------------------------------------------------------------------------
// Name: ExtensionTypes::Purge
// Created: JSR 2010-10-01
// -----------------------------------------------------------------------------
void ExtensionTypes::Purge()
{
    tools::StringResolver< DictionaryType >::DeleteAll();
    tools::StringResolver< ExtensionType >::DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: ExtensionTypes::ReadElement
// Created: JSR 2010-10-01
// -----------------------------------------------------------------------------
void ExtensionTypes::ReadElement( const std::string& name, xml::xistream& xis )
{
    if( name == "dictionaries" )
        xis >> xml::list( "dictionary", *this, &ExtensionTypes::ReadDictionary );
    else
        tools::StringResolver< ExtensionType >::Register( name, *new ExtensionType( xis ) );
}

// -----------------------------------------------------------------------------
// Name: ExtensionTypes::ReadDictionary
// Created: JSR 2010-10-01
// -----------------------------------------------------------------------------
void ExtensionTypes::ReadDictionary( xml::xistream& xis )
{
    tools::StringResolver< DictionaryType >::Register( xis.attribute< std::string >( "name" ), *new DictionaryType( xis ) );
}

// -----------------------------------------------------------------------------
// Name: ExtensionTypes::GetNameByType
// Created: ABR 2011-05-03
// -----------------------------------------------------------------------------
const std::string ExtensionTypes::GetNameByType( AttributeType::EType type ) const
{
    ExtensionType* extension = tools::StringResolver< ExtensionType >::Find( "orbat-attributes" );
    if( extension )
    {
        tools::Iterator< const AttributeType& > it = extension->CreateIterator();
        while( it.HasMoreElements() )
        {
            const AttributeType& attribute = it.NextElement();
            if( attribute.GetType() == type )
                return attribute.GetName();
        }
    }
    return "";
}
