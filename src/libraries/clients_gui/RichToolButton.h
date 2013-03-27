// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __RichToolButton_h_
#define __RichToolButton_h_

namespace gui
{

// =============================================================================
/** @class  RichToolButton
    @brief  RichToolButton
*/
// Created: NPT 2013-03-14
// =============================================================================
class RichToolButton : public QToolButton
{

public:
    //! @name Constructors/Destructor
    //@{
    RichToolButton( const QString& objectName, QWidget* parent = 0, const QString& text = 0 );
    virtual ~RichToolButton();
    //@}
};

}

#endif // __RichToolButton_h_
