// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __gui_RichCheckBox_h_
#define __gui_RichCheckBox_h_

#include "RichWidget.h"
#include "QtGui/qcheckbox.h"

namespace gui
{
// =============================================================================
/** @class  RichCheckBox
    @brief  RichCheckBox
*/
// Created: NPT 2013-03-12
// =============================================================================
class RichCheckBox : public RichWidget< QCheckBox >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit RichCheckBox( const QString& objectName, QWidget* parent = 0 );
             RichCheckBox( const QString& objectName, const QString& text, QWidget* parent = 0 );
    virtual ~RichCheckBox();
    //@}
};

} //! namespace gui

#endif // __gui_RichCheckBox_h_
