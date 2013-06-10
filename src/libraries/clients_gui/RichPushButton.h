// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __RichPushButton_h_
#define __RichPushButton_h_

#include "RichWidget.h"
#include "QtGui/qpushbutton.h"

namespace gui
{
// =============================================================================
/** @class  RichPushButton
    @brief  RichPushButton
*/
// Created: NPT 2013-03-12
// =============================================================================
class RichPushButton : public RichWidget< QPushButton >
{

public:
    //! @name Constructors/Destructor
    //@{
             RichPushButton( const QString& objectName, const QString& text, QWidget* parent = 0 );
             RichPushButton( const QString& objectName, const QIcon & icon, const QString& text, QWidget* parent = 0 );
    virtual ~RichPushButton();
    //@}
};

} //! namespace gui

#endif // __RichPushButton_h_
