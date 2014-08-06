// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "clients_kernel_test_pch.h"

#include "clients_kernel/Controllers.h"
#include "clients_kernel/SafePointer.h"

#include <QtGui/QApplication>

BOOST_AUTO_TEST_CASE( TestSafePointerIsDeleted )
{
    int dummy = 0;
    QApplication app( dummy, nullptr );
    kernel::Controllers controllers;
    controllers.controller_.Create( dummy );
    kernel::SafePointer< int > ptr( controllers, &dummy );
    kernel::SafePointer< int > ptr2( controllers );
    ptr2 = ptr;
    const auto ptr3( ptr );
    const auto ptr4 = ptr;
    controllers.controller_.Delete( dummy );
    BOOST_CHECK( !ptr );
    BOOST_CHECK( !ptr2 );
    BOOST_CHECK( !ptr3 );
    BOOST_CHECK( !ptr4 );
}