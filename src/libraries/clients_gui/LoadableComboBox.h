// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LoadableComboBox_h_
#define __LoadableComboBox_h_

#include "LoadableField.h"

namespace gui
{

// =============================================================================
/** @class  LoadableComboBox
    @brief  Combo box that can be filled from a file or manually.
*/
// Created: JSR 2011-07-07
// =============================================================================
class LoadableComboBox : public LoadableFieldTemplate< RichWidget< QComboBox > >
{
public:
    //! @name Constructors/Destructor
    //@{
    LoadableComboBox( const QString& objectName, bool rw, QWidget* parent );
    virtual ~LoadableComboBox();
    //@}

    //! @name Operations
    //@{
    QString currentText() const;
    //@}
};

}

#endif // __LoadableComboBox_h_
