// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "SessionFactory.h"
#include "FileSystem_ABC.h"
#include "PortFactory_ABC.h"
#include "Session.h"

#include "cpplog/cpplog.hpp"
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
SessionFactory::SessionFactory( cpplog::BaseLogger& log, const runtime::Runtime_ABC& runtime, const UuidFactory_ABC& uuids,
                                const FileSystem_ABC& system, PortFactory_ABC& ports, const boost::filesystem::path& data,
                                const boost::filesystem::path& applications, Pool_ABC& pool )
    : log_         ( log )
    , runtime_     ( runtime )
    , uuids_       ( uuids )
    , system_      ( system )
    , data_        ( data )
    , applications_( applications )
    , ports_       ( ports )
    , pool_        ( pool )
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
boost::shared_ptr< Session_ABC > SessionFactory::Create( const boost::uuids::uuid& node, const std::string& exercise, const std::string& name ) const
{
    return boost::shared_ptr< Session_ABC >( new Session( log_, pool_, runtime_, uuids_, system_, data_, applications_, node, exercise, name, ports_ ) );
}

// -----------------------------------------------------------------------------
// Name: SessionFactory::Reload
// Created: BAX 2012-03-21
// -----------------------------------------------------------------------------
SessionFactory_ABC::T_Sessions SessionFactory::Reload() const
{
    SessionFactory_ABC::T_Sessions sessions;
    BOOST_FOREACH( const boost::filesystem::path& path, system_.Glob( data_ / L"exercises", L"session.id" ) )
        try
        {
            xml::xistringstream xis( system_.ReadFile( path ) );
            boost::shared_ptr< Session_ABC > ptr = boost::make_shared< Session >( boost::ref( log_ ), pool_, runtime_, system_, data_, applications_, boost::ref( xis ), boost::ref( ports_ ) );
            sessions.insert( std::make_pair( ptr->GetTag(), ptr ) );
        }
        catch( const std::exception& err )
        {
            LOG_WARN( log_ ) << "[session] Unable to reload session from " << path.string() << " - " << err.what();
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
    const boost::filesystem::path root = data_ / L"exercises";
    const size_t offset = root.string().size() + 1;
    BOOST_FOREACH( boost::filesystem::path path, system_.Glob( root, L"exercise.xml" ) )
    {
        std::wstring leaf = path.remove_filename().wstring();
        leaf = leaf.substr( offset, leaf.size() - offset );
        std::replace( leaf.begin(), leaf.end(), L'\\', L'/' );
        exercises.push_back( runtime::Utf8Convert( leaf ) );
    }
    return exercises;
}
