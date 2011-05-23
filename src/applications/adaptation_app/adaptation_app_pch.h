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
#include <qaction.h>
#include <qapp.h>
#include <qapplication.h>
#include <qbrush.h>
#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qcolor.h>
#include <qcombo.h>
#include <qcombobox.h>
#include <qcstring.h>
#include <qcursor.h>
#include <qdialog.h>
#include <qdict.h>
#include <qdragobject.h>
#include <qeventloop.h>
#include <qfiledialog.h>
#include <qfont.h>
#include <qfontmetrics.h>
#include <qframe.h>
#include <qgrid.h>
#include <qgroupbox.h>
#include <qhbox.h>
#include <qheader.h>
#include <qhgroupbox.h>
#include <qiconset.h>
#include <qimage.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qlistbox.h>
#include <qlistview.h>
#include <qmainwindow.h>
#include <qmap.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qmime.h>
#include <qobject.h>
#include <qpaintdevicemetrics.h>
#include <qpainter.h>
#include <qpalette.h>
#include <qpen.h>
#include <qpixmap.h>
#include <qpoint.h>
#include <qpointarray.h>
#include <qpopmenu.h>
#include <qpopupmenu.h>
#include <qprinter.h>
#include <qprocess.h>
#include <qprogressbar.h>
#include <qptrlist.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qrect.h>
#include <qsettings.h>
#include <qsplashscreen.h>
#include <qstatusbar.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qstyle.h>
#include <qtable.h>
#include <qtabwidget.h>
#include <qtextcodec.h>
#include <qtextedit.h>
#include <qtextview.h>
#include <qtimer.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qtooltip.h>
#include <qtranslator.h>
#include <qvalidator.h>
#include <qvalidator.h>
#include <qvaluelist.h>
#include <qvbox.h>
#include <qvgroupbox.h>
#include <qwhatsthis.h>
#include <qwidget.h>
#include <qwizard.h>
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
