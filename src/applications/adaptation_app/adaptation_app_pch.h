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
#include <QtCore/qtextcodec.h>
#include <QtCore/qtranslator.h>
#include <Qtcore/qtimer.h>

#include <Qt3Support/q3buttongroup.h>
#include <Qt3Support/q3cstring.h>
#include <Qt3Support/q3dict.h>
#include <Qt3Support/q3dragobject.h>
#include <Qt3Support/q3filedialog.h>
#include <Qt3Support/q3frame.h>
#include <Qt3Support/q3grid.h>
#include <Qt3Support/q3groupbox.h>
#include <Qt3Support/q3hbox.h>
#include <Qt3Support/q3header.h>
#include <Qt3Support/q3hgroupbox.h>
#include <Qt3Support/q3listbox.h>
#include <Qt3Support/q3listview.h>
#include <Qt3Support/q3paintdevicemetrics.h>
#include <Qt3Support/q3pointarray.h>
#include <Qt3Support/q3popupmenu.h>
#include <Qt3Support/q3process.h>
#include <Qt3Support/q3progressbar.h>
#include <Qt3Support/q3ptrlist.h>
#include <Qt3Support/q3table.h>
#include <Qt3Support/q3textedit.h>
#include <Qt3Support/q3textview.h>
#include <Qt3Support/q3toolbar.h>
#include <Qt3Support/q3valuelist.h>
#include <Qt3Support/q3vbox.h>
#include <Qt3Support/q3vgroupbox.h>
#include <Qt3Support/q3whatsthis.h>
#include <Qt3Support/q3wizard.h>
#include <Qt3Support/q3frame.h>
#include <Qt3Support/q3hgroupbox.h>
#include <Qt3Support/q3vbox.h>
#include <Qt3Support/q3grid.h>
#include <Qt3Support/q3boxlayout.h>
#include <Qt3Support/q3gridlayout.h>

#pragma warning( pop )

#include "MT_Tools/MT_Assert.h"
#include "MT_Tools/MT_Logger.h"
#include "ADN_Type_ABC.h"
#include <xeumeuleu/xml.hpp>

namespace xml
{
    template< typename T >
    class attribute_manipulator< ADN_Type_ABC< T > >
    {
    public:
        //! @name Constructors/Destructor
        //@{
        attribute_manipulator( const std::string& name, ADN_Type_ABC< T >& value )
            : name_ ( name )
            , value_( value )
        {}
        //@}

        //! @name Operators
        //@{
        friend xistream& operator>>( xml::xistream& xis, const attribute_manipulator& m )
        {
            T value = m.value_.GetData();
            xis >> xml::attribute( m.name_, value );
            m.value_ = value;
            return xis;
        }
        friend xostream& operator<<( xostream& xos, const attribute_manipulator& m )
        {
            T value = m.value_.GetData();
            xos << xml::attribute( m.name_, value );
            return xos;
        }
        //@}

    private:
        //! @name Copy/Assignment
        //@{
        attribute_manipulator& operator=( const attribute_manipulator& ); //!< Assignment operator
        //@}

    private:
        //! @name Member data
        //@{
        std::string name_;
        ADN_Type_ABC< T >& value_;
        //@}
    };

    template< typename T >
    class content_manipulator< ADN_Type_ABC< T > >
    {
    public:
        //! @name Constructors/Destructor
        //@{
        content_manipulator( const std::string& tag, ADN_Type_ABC< T >& value )
            : tag_  ( tag )
            , value_( value )
        {}
        //@}

        //! @name Operators
        //@{
        friend xistream& operator>>( xml::xistream& xis, const content_manipulator& m )
        {
            T value;
            xis >> xml::start( m.tag_ ) >> value >> xml::end;
            m.value_ = value;
            return xis;
        }
        friend xostream& operator<<( xostream& xos, const content_manipulator& m )
        {
            return xos << xml::start( m.tag_ ) << m.value_.GetData() << xml::end;
        }
        //@}

    private:
        //! @name Copy/Assignment
        //@{
        content_manipulator& operator=( const content_manipulator& ); //!< Assignment operator
        //@}

    private:
        //! @name Member data
        //@{
        std::string tag_;
        ADN_Type_ABC< T >& value_;
        //@}
    };
}

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
