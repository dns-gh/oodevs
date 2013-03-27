// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __RichCheckBox_h_
#define __RichCheckBox_h_

#include <boost/noncopyable.hpp>
#include "QtGui/qcheckbox.h"

namespace gui
{
// =============================================================================
/** @class  RichCheckBox
    @brief  RichCheckBox
*/
// Created: NPT 2013-03-12
// =============================================================================
class RichCheckBox : public QCheckBox
                   , private boost::noncopyable
    {

public:
    //! @name Constructors/Destructor
    //@{
    RichCheckBox( const QString& objectName );
    RichCheckBox( const QString& objectName, const QString& text, QWidget* parent );
    virtual ~RichCheckBox();
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

} //! namespace gui

#endif // __RichCheckBox_h_
