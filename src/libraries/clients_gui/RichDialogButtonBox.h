// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __RichDialogButtonBox_h_
#define __RichDialogButtonBox_h_

#include "RichWidget.h"

namespace gui
{

// =============================================================================
/** @class  RichDialogButtonBox
    @brief  RichDialogButtonBox
*/
// Created: NPT 2013-03-22
// =============================================================================
class RichDialogButtonBox : public RichWidget< QDialogButtonBox >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit RichDialogButtonBox( const QString& objectname, QWidget* parent = 0 );
             RichDialogButtonBox( const QString& objectname, StandardButtons buttonType, QWidget* parent = 0 );
    virtual ~RichDialogButtonBox();
    //@}
};

} //! namespace gui

#endif // __RichDialogButtonBox_h_
