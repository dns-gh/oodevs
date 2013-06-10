// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __RichRadioButton_h_
#define __RichRadioButton_h_

#include "RichWidget.h"

namespace gui
{

// =============================================================================
/** @class  RichRadioButton
    @brief  RichRadioButton
*/
// Created: NPT 2013-03-14
// =============================================================================
class RichRadioButton : public RichWidget< QRadioButton >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit RichRadioButton( const QString& objectName, QWidget* parent  = 0 );
             RichRadioButton( const QString& objectName, const QString& text, QWidget* parent  = 0 );
    virtual ~RichRadioButton();
    //@}
};

} //! namespace gui

#endif // __RichRadioButton_h_
