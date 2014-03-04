// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************
#ifndef __StubTER_World_h_
#define __StubTER_World_h_

#include <boost/shared_ptr.hpp>

class TER_World;

class FakeWorld
{
public:
    explicit FakeWorld( const std::string& exercise );
    virtual ~FakeWorld();
};

boost::shared_ptr< TER_World > CreateWorld( const std::string& exercise );

#endif //__StubTER_World_h_
