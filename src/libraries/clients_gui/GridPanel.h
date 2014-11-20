// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __GridPanel_h_
#define __GridPanel_h_

#include "PreferencePanel_ABC.h"

namespace kernel
{
    class OptionsController;
}

namespace gui
{

// =============================================================================
/** @class  GridPanel
    @brief  GridPanel
*/
// Created: JSR 2014-11-19
// =============================================================================
class GridPanel : public PreferencePanel_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             GridPanel( QWidget* parent, kernel::OptionsController& options );
    virtual ~GridPanel();
    //@}

private:
    //! @name Member data
    //@{
    kernel::OptionsController& options_;
    //@}
};

} //! namespace gui

#endif // __GridPanel_h_
