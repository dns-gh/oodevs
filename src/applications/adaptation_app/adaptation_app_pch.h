// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_APPLICATION_PCH_H_
#define __ADN_APPLICATION_PCH_H_

#pragma warning( disable : 4231 4355 4996 )

#pragma warning( push, 0 )

#include <QtGui/QtGui>
#include <QtGui/qtabwidget.h>

#include <QtWebKit/qwebview.h>

#include <QtOpenGL/qgl.h>

#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qeventloop.h>
#include <QtCore/qmap.h>
#include <QtCore/qobject.h>
#include <QtCore/qpoint.h>
#include <QtCore/qrect.h>
#include <QtCore/qsettings.h>
#include <QtCore/qtranslator.h>
#include <Qtcore/qtimer.h>

#include <Qt3Support/q3buttongroup.h>
#include <Qt3Support/q3frame.h>
#include <Qt3Support/q3grid.h>
#include <Qt3Support/q3groupbox.h>
#include <Qt3Support/q3hbox.h>
#include <Qt3Support/q3hgroupbox.h>
#include <Qt3Support/q3listbox.h>
#include <Qt3Support/q3paintdevicemetrics.h>
#include <Qt3Support/q3pointarray.h>
#include <Qt3Support/q3popupmenu.h>
#include <Qt3Support/q3process.h>
#include <Qt3Support/q3progressbar.h>
#include <Qt3Support/q3textedit.h>
#include <Qt3Support/q3textview.h>
#include <Qt3Support/q3boxlayout.h>
#include <Qt3Support/q3vgroupbox.h>
#include <Qt3Support/q3hgroupbox.h>
#include <Qt3Support/q3vbox.h>
#include <Qt3Support/q3grid.h>
#include <Qt3Support/q3gridlayout.h>

#pragma warning( pop )

#undef min
#undef max

#include <tools/Exception.h>
#include <tools/Path.h>
#include "tools/FileWrapper.h"
#include "tools/SchemaWriter.h"
#include "tools/XmlStreamOperators.h"
#include <cassert>
#include "MT_Tools/MT_Logger.h"
#include "ADN_XmlStreamOperators.h"
#include "clients_gui/ObjectNameManager.h"

struct DeleteOwnedObject
{
    template< class T >
    void operator()( const T* ptr )
    {
        delete ptr;
    }
};

template< class T_Cont >
void clear_owned_ptrs( T_Cont& container )
{
    std::for_each( container.begin(), container.end(), DeleteOwnedObject() );
    container.clear();
}

inline std::string& trim( std::string& str )
{
    static const std::string spaces = " \t\n\r";
    str.erase( 0, str.find_first_not_of( spaces ) );
    str.resize( str.find_last_not_of( spaces ) + 1 );
    return str;
}

#endif // __ADN_APPLICATION_PCH_H_
