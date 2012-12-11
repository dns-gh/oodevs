// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __frontend_pch_h_
#define __frontend_pch_h_

#pragma warning( disable: 4996 ) // Function call with parameters that may be unsafe
#define _WIN32_WINNT 0x0501
#include <exception>
#include <memory>
#pragma warning( push, 0 )
#include <QtCore/qstring.h>
#include <QtGui/QEvent.h>
#include <QtGui/qtabwidget.h>
#include <QtGui/qlistwidget.h>
#include <QtGui/qlayout.h> 
#include <QtGui/qtreeview.h>
#include <QtGui/qstandarditemmodel.h>
#pragma warning( pop )
#include <string>

#include <tools/Exception.h>

#endif // __frontend_pch_h_
