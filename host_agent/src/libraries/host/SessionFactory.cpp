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
#include "Session.h"

#include <boost/make_shared.hpp>

using namespace host;

// -----------------------------------------------------------------------------
// Name: SessionFactory::SessionFactory
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
SessionFactory::SessionFactory( const runtime::Runtime_ABC& runtime, const UuidFactory_ABC& uuids, const FileSystem_ABC& system )
    : runtime_( runtime )
    , uuids_  ( uuids )
    , system_ ( system )
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
boost::shared_ptr< Session_ABC > SessionFactory::Create( const std::string& exercise, const SessionConfig& config ) const
{
    return boost::make_shared< Session >( runtime_, uuids_, system_, exercise, config );
}
