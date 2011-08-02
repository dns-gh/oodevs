// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LoadableTimeEdit_h_
#define __LoadableTimeEdit_h_

#include "LoadableField.h"

namespace gui
{

// =============================================================================
/** @class  LoadableTimeEdit
    @brief  Time edit widget that can be filled from a file or manually.
*/
// Created: BCI 2011-05-09
// =============================================================================
class LoadableTimeEdit : public LoadableFieldTemplate< Q3TimeEdit >
{
public:

    //! @name Constructors/Destructor
    //@{
             LoadableTimeEdit( QWidget* parent, const QString& probableName = QString() );
    virtual ~LoadableTimeEdit();
    //@}

    //! @name Operations
    //@{
    QTime time() const;
    //@}
};

} // end namespace gui

#endif // __LoadableTimeEdit_h_
