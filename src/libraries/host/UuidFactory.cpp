// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "UuidFactory.h"

#include <boost/uuid/uuid_generators.hpp>

using namespace host;

// -----------------------------------------------------------------------------
// Name: UuidFactory::UuidFactory
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
UuidFactory::UuidFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UuidFactory::~UuidFactory
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
UuidFactory::~UuidFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UuidFactory::GetRandom
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
Uuid UuidFactory::Create() const
{
    return boost::uuids::random_generator()();
}
