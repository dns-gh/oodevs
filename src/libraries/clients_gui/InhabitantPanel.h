// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef gui_InhabitantPanel_h
#define gui_InhabitantPanel_h

#include "PreferencePanel_ABC.h"

namespace kernel
{
    class OptionsController;
}

namespace gui
{

    // =============================================================================
/** @class  InhabitantPanel
    @brief  Inhabitant panel
*/
// Created: LGY 2010-12-31
// =============================================================================
class InhabitantPanel : public PreferencePanel_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             InhabitantPanel( QWidget* parent, kernel::OptionsController& options );
    virtual ~InhabitantPanel();
    //@}
};

} //! namespace gui

#endif // gui_InhabitantPanel_h
