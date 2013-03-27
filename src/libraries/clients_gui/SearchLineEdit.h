// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __SearchLineEdit_h_
#define __SearchLineEdit_h_

#include "RichLineEdit.h"

namespace gui
{

// =============================================================================
/** @class  SearchLineEdit
    @brief  SearchLineEdit
*/
// Created: ABR 2012-03-27
// =============================================================================
class SearchLineEdit : public RichLineEdit
{
public:
    //! @name Constructors/Destructor
    //@{
    SearchLineEdit( const QString& objectName, QWidget* parent = 0 );
    virtual ~SearchLineEdit();
    //@}
};

} //! namespace gui

#endif // __SearchLineEdit_h_
