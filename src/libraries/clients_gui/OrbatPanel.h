// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __gui__OrbatPanel_h_
#define __gui__OrbatPanel_h_

#include "clients_gui/PreferencePanel_ABC.h"

namespace kernel
{
    class OptionsController;
}

namespace gui
{

// =============================================================================
/** @class  OrbatPanel
    @brief  Orbat panel
*/
// Created: LGY 2012-03-22
// =============================================================================
class OrbatPanel : public gui::PreferencePanel_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             OrbatPanel( QWidget* parent, kernel::OptionsController& options );
    virtual ~OrbatPanel();
    //@}
};

} //! namespace gui

#endif // __gui__OrbatPanel_h_
