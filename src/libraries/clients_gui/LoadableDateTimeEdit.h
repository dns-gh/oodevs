// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __LoadableDateTimeEdit_h_
#define __LoadableDateTimeEdit_h_

#include "LoadableField.h"

namespace gui
{

// =============================================================================
/** @class  LoadableDateTimeEdit
    @brief  DateTime edit widget that can be filled from a file or manually.
*/
// Created: JSR 2013-09-12
// =============================================================================
class LoadableDateTimeEdit : public LoadableFieldTemplate< QDateTimeEdit >
{
public:

    //! @name Constructors/Destructor
    //@{
             LoadableDateTimeEdit( const QString& objectName, QWidget* parent );
    virtual ~LoadableDateTimeEdit();
    //@}

    //! @name Operations
    //@{
    QDateTime dateTime() const;
    //@}
};

} // end namespace gui

#endif // __LoadableDateTimeEdit_h_
