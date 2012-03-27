// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifdef  _MSC_VER
#   define _SCL_SECURE_NO_WARNINGS
#endif

#include "SessionFactory.h"
#include "PortFactory_ABC.h"
#include "FileSystem_ABC.h"
#include "Session.h"
#include "runtime/Utf8.h"

#include <xeumeuleu/xml.hpp>

#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include <boost/ref.hpp>

using namespace host;

// -----------------------------------------------------------------------------
// Name: SessionFactory::SessionFactory
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
SessionFactory::SessionFactory( const runtime::Runtime_ABC& runtime, const UuidFactory_ABC& uuids, const FileSystem_ABC& system, PortFactory_ABC& ports,
                                const boost::filesystem::wpath& data, const boost::filesystem::wpath& applications )
    : runtime_     ( runtime )
    , uuids_       ( uuids )
    , system_      ( system )
    , ports_       ( ports )
    , data_        ( data )
    , applications_( applications )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SessionFactory::~SessionFactory
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
SessionFactory::~SessionFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SessionFactory::Create
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
boost::shared_ptr< Session_ABC > SessionFactory::Create( const std::string& exercise, const std::string& name ) const
{
    return boost::make_shared< Session>( runtime_, uuids_, system_, data_, applications_, exercise, name, boost::ref( ports_ ) );
}

// -----------------------------------------------------------------------------
// Name: SessionFactory::Reload
// Created: BAX 2012-03-21
// -----------------------------------------------------------------------------
SessionFactory_ABC::T_Sessions SessionFactory::Reload() const
{
    SessionFactory_ABC::T_Sessions sessions;
    BOOST_FOREACH( const boost::filesystem::wpath& path, system_.Glob( data_ / L"exercises", L"session.tag" ) )
        try
        {
            xml::xistringstream xis( system_.ReadFile( path ) );
            boost::shared_ptr< Session_ABC > ptr = boost::make_shared< Session >( runtime_, system_, data_, applications_, boost::ref( xis ), boost::ref( ports_ ) );
            sessions.insert( std::make_pair( ptr->GetTag(), ptr ) );
        }
        catch( const std::exception& )
        {
            continue; // skip invalid session
        }
    return sessions;
}

// -----------------------------------------------------------------------------
// Name: SessionFactory::GetExercises
// Created: BAX 2012-03-27
// -----------------------------------------------------------------------------
std::vector< std::string > SessionFactory::GetExercises() const
{
    std::vector< std::string > exercises;
    const boost::filesystem::wpath root = data_ / L"exercises";
    size_t offset = root.string().size() + 1;
    BOOST_FOREACH( boost::filesystem::wpath path, system_.Glob( root, L"exercise.xml" ) )
    {
        const std::wstring leaf = path.remove_filename().string();
        exercises.push_back( runtime::Utf8Convert( leaf.substr( offset, leaf.size() - offset - 1 ) ) );
    }
    return exercises;
}