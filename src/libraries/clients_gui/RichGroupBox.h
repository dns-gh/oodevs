// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __RichGroupBox_h_
#define __RichGroupBox_h_

#include "RichWarnWidget.h"

namespace gui
{

// =============================================================================
/** @class  RichGroupBox
    @brief  RichGroupBox
*/
// Created: ABR 2011-11-21
// =============================================================================
class RichGroupBox : public RichWarnWidget< QGroupBox >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit RichGroupBox( const QString& objectName, QWidget* parent = 0 );
             RichGroupBox( const QString& objectName, const QString& title, QWidget* parent = 0 );
    virtual ~RichGroupBox();
    //@}
};

} //! namespace gui

#endif // __RichGroupBox_h_
