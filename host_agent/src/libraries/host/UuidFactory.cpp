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
boost::uuids::uuid UuidFactory::GetRandom() const
{
    return boost::uuids::random_generator()();
}
