// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __integration_decisionnal_test_pch_h_
#define __integration_decisionnal_test_pch_h_

#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

#define BOOST_TODO BOOST_WARN_MESSAGE( 0, "TODO !" )

std::string BOOST_RESOLVE( const std::string& filename );

#ifndef uint
# define uint unsigned int
#endif

#ifndef PLATFORM
#define PLATFORM _vc80
#endif

#ifndef PLUGIN
#ifdef NDEBUG
#define PLUGIN46( plugin ) "'plugin_" + plugin + BOOST_PP_STRINGIZE( PLATFORM ) + "-mt-4_6.plugin',"
#define PLUGIN( plugin ) "'plugin_" + plugin + BOOST_PP_STRINGIZE( PLATFORM ) + "-mt.plugin',"
#else
#define PLUGIN46( plugin ) "'plugin_" + plugin + BOOST_PP_STRINGIZE( PLATFORM ) + "-mt-gd-4_6.plugin',"
#define PLUGIN( plugin ) "'plugin_" + plugin + BOOST_PP_STRINGIZE( PLATFORM ) + "-mt-gd.plugin',"
#endif
#endif

#define BRAIN_INIT() std::string( "plugins={" ) + PLUGIN( "masalife_brain" ) + PLUGIN( "services" ) + PLUGIN( "knowledge" ) + "} cwd='" + BOOST_RESOLVE( "." ) + "'"

#endif // __integration_decisionnal_test_pch_h_
