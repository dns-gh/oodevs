// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "host_test.h"

#include "host/UuidFactory.h"

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/xpressive/xpressive.hpp>

BOOST_AUTO_TEST_CASE( uuid_factory_returns_valid_uuids )
{
    const std::string x = "[0-9a-fA-F]";
    const boost::xpressive::sregex session_regex = boost::xpressive::sregex::compile(
        x +"{8,8}-" + x + "{4,4}-" + x + "{4,4}-" + x + "{4,4}-" + x + "{12,12}" );
    host::UuidFactory uuids;
    const std::string uuid = boost::lexical_cast< std::string >( uuids.Create() );
    bool valid = boost::xpressive::regex_match( uuid, session_regex );
    BOOST_CHECK( valid );
}
