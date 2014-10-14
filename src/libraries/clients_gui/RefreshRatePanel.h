// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __gui__RefreshRatePanel_h_
#define __gui__RefreshRatePanel_h_

#include "PreferencePanel_ABC.h"

namespace kernel
{
    class OptionsController;
}

namespace gui
{

// =============================================================================
/** @class  RefreshRatePanel
    @brief  RefreshRatePanel
*/
// Created: LDC 2014-02-21
// =============================================================================
class RefreshRatePanel : public PreferencePanel_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             RefreshRatePanel( QWidget* parent, kernel::OptionsController& options );
    virtual ~RefreshRatePanel();
    //@}
};

} //! namespace gui

#endif // __gui__RefreshRatePanel_h_
