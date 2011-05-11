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

namespace gui
{

// =============================================================================
/** @class  LoadableSpinBox
    @brief  Spin box that can be filled from a file or manually.
*/
// Created: BCI 2011-05-09
// =============================================================================
class LoadableSpinBox : public LoadableFieldTemplate< QSpinBox >
{
public:

    //! @name Constructors/Destructor
    //@{
             LoadableSpinBox( int minValue, int maxValue, int step, QWidget* parent, const QString& probableName = QString() );
    virtual ~LoadableSpinBox();
    //@}

    //! @name Operations
    //@{
    int value() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LoadableSpinBox( const LoadableSpinBox& );            //!< Copy constructor
    LoadableSpinBox& operator=( const LoadableSpinBox& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

} // end namespace gui

#endif // __LoadableSpinBox_h_
