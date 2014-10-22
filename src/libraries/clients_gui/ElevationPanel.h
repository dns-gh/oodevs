// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ElevationPanel_h_
#define __ElevationPanel_h_

#include "PreferencePanel_ABC.h"

namespace kernel
{
    class DetectionMap;
    class OptionsController;
}

namespace gui
{
    class GradientPreferencesEditor;

// =============================================================================
/** @class  ElevationPanel
    @brief  ElevationPanel
*/
// Created: AGE 2007-01-17
// =============================================================================
class ElevationPanel : public PreferencePanel_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ElevationPanel( QWidget* parent,
                             kernel::OptionsController& options,
                             const kernel::DetectionMap& detection );
    virtual ~ElevationPanel();
    //@}

    //! @name Operations
    //@{
    virtual void Load( const GlProxy& );
    //@}

private:
    //! @name Member data
    //@{
    GradientPreferencesEditor* gradientEditor_;
    //@}
};

} //! namespace gui

#endif // __ElevationPanel_h_
