// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LoadableCheckBox_h_
#define __LoadableCheckBox_h_

#include "LoadableField.h"

namespace gui
{

// =============================================================================
/** @class  LoadableCheckBox
    @brief  Check box that can be filled from a file or manually.
*/
// Created: BCI 2011-05-09
// =============================================================================
class LoadableCheckBox : public LoadableFieldTemplate< QCheckBox >
{
public:

    //! @name Constructors/Destructor
    //@{
             LoadableCheckBox( const QString& text, QWidget* parent, const QString& probableName = QString() );
    virtual ~LoadableCheckBox();
    //@}

    //! @name Operations
    //@{
    bool isChecked() const;
    //@}
};

} // end namespace gui

#endif // __LoadableCheckBox_h_
