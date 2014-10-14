// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SizeButton_h_
#define __SizeButton_h_

#include "OptionWidgets.h"

namespace kernel
{
    class OptionsController;
}

namespace gui
{

// =============================================================================
/** @class  SizeButton
    @brief  Size button
*/
// Created: SBO 2006-04-05
// =============================================================================
class SizeButton : public OptionWidget< RichWidget< QSlider > >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             SizeButton( kernel::OptionsController& options,
                         const QString& objectName,
                         const std::string& optionName,
                         float min,
                         float max,
                         float value,
                         QWidget* parent = 0 );
    virtual ~SizeButton();
    //@}

    //! @name Operations
    //@{
    void SetSize( float value );
    float GetSize() const;
    //@}
};

} //! namespace gui

#endif // __SizeButton_h_
