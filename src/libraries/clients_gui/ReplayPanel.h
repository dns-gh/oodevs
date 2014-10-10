// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __gui__ReplayPanel_h
#define __gui__ReplayPanel_h

#include "PreferencePanel_ABC.h"

namespace kernel
{
    class OptionsController;
}

namespace gui
{

// =============================================================================
/** @class  ReplayPanel
    @brief  ReplayPanel
*/
// Created: SLI 2014-06-06
// =============================================================================
class ReplayPanel : public PreferencePanel_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ReplayPanel( QWidget* parent, kernel::OptionsController& options );
    virtual ~ReplayPanel();
    //@}
};

} //! namespace gui

#endif // __gui__ReplayPanel_h
