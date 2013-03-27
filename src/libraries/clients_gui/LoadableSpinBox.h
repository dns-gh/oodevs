// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LoadableSpinBox_h_
#define __LoadableSpinBox_h_

#include "LoadableField.h"
#include "RichSpinBox.h"

namespace gui
{

// =============================================================================
/** @class  LoadableSpinBox
    @brief  Spin box that can be filled from a file or manually.
*/
// Created: BCI 2011-05-09
// =============================================================================
class LoadableSpinBox : public LoadableFieldTemplate< RichSpinBox >
{
public:

    //! @name Constructors/Destructor
    //@{
             LoadableSpinBox( const QString& objectName, int minValue, int maxValue, int step, QWidget* parent );
    virtual ~LoadableSpinBox();
    //@}

    //! @name Operations
    //@{
    int value() const;
    void setSuffix( const QString& );
    void setSpecialValueText( const QString& );
    void setValue( int );
    //@}
};

} // end namespace gui

#endif // __LoadableSpinBox_h_
