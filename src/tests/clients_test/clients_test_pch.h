// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __clients_test_pch_h_
#define __clients_test_pch_h_

#include <qstring.h>

#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

#define BOOST_TODO BOOST_WARN_MESSAGE( 0, "TODO !" )

#define MOCKPP_ENABLE_DEFAULT_FORMATTER
#include <mockpp/chaining/CountedChainableMethod.h>
#include <mockpp/chaining/ChainingMockObjectSupport.h>

#pragma warning( push, 1 )
#pragma warning( disable : 4505 )
#include <turtle/mock.hpp>
#pragma warning( pop )

std::string BOOST_RESOLVE( const std::string& filename );

#ifndef uint
# define uint unsigned int
#endif

#pragma warning( push, 0 )
#include <qapplication.h>
#include <qdatetime.h>
#include <qfont.h>
#include <qgl.h>
#include <qpopupmenu.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qwidget.h>
#pragma warning( pop )

#endif // __clients_test_pch_h_
