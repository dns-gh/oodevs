// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifdef  _MSC_VER
#define _SCL_SECURE_NO_WARNINGS
#endif

#include "Session.h"

#include <runtime/Process_ABC.h>

#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace host;

// -----------------------------------------------------------------------------
// Name: Session::Session
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
Session::Session( const SessionConfig& config )
    : tag_   ( boost::uuids::random_generator()() )
    , config_( config )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Session::~Session
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
Session::~Session()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Session::GetTag
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
boost::uuids::uuid Session::GetTag() const
{
    return tag_;
}

// -----------------------------------------------------------------------------
// Name: Session::ToJson
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
std::string Session::ToJson() const
{
    const int pid = process_ ? process_->GetPid() : -1;
    const std::string& name = process_ ? process_->GetName() : "";
    return (boost::format( "{ \"tag\" : \"%1%\", \"pid\" : %2%, \"name\" : \"%3%\" }" )
        % boost::lexical_cast< std::string >( tag_ ) % pid % name ).str();
}

// -----------------------------------------------------------------------------
// Name: Session::Start
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
void Session::Start()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Session::Stop
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
void Session::Stop()
{
    // NOTHING
}