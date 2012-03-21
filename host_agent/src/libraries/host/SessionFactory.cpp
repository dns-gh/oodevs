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
#include "Session.h"

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
    std::auto_ptr< Port_ABC > port = ports_.Create();
    return boost::make_shared< Session>( runtime_, uuids_, system_, data_, applications_, exercise, name, boost::ref( port ) );
}
