// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __RichTabWidget_h_
#define __RichTabWidget_h_

#include <boost/noncopyable.hpp>
#include <QtGui/qtabwidget.h>

namespace gui
{

// =============================================================================
/** @class  RichTabWidget
    @brief  RichTabWidget
*/
// Created: NPT 2013-03-14
// =============================================================================
class RichTabWidget : public QTabWidget
                    , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             RichTabWidget( const QString& objectName , QWidget* parent = 0 );
    virtual ~RichTabWidget();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

} //end namespace gui

#endif // __RichTabWidget_h_
