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
#include "clients_kernel/OptionsObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class OptionsController;
}

namespace gui
{
    class CheckBox;
    class Elevation2dLayer;
    class Gradient;
    class GradientPreferences;
    class GradientWidget;
    class Painter_ABC;
    class RichGroupBox;
}

namespace gui
{
// =============================================================================
/** @class  ElevationPanel
    @brief  ElevationPanel
*/
// Created: AGE 2007-01-17
// =============================================================================
class ElevationPanel : public PreferencePanel_ABC
                     , public tools::Observer_ABC
                     , public kernel::OptionsObserver_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ElevationPanel( QWidget* parent,
                             const std::shared_ptr< Elevation2dLayer >& layer,
                             kernel::Controllers& controllers,
                             const Painter_ABC& painter );
    virtual ~ElevationPanel();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    virtual void Reset();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnGradientChanged( Gradient& gradient );
    void OnEnableVariableGradient( bool );
    void OnEnableHillshade( bool );
    void OnHillShadeDirection( int );
    void OnStrengthChanged( int value );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::OptionsController& options_;
    std::shared_ptr< Elevation2dLayer > layer_;
    GradientPreferences& preferences_;
    RichGroupBox* hsBox_;
    CheckBox* fitColorGradienttoViewPort_;
    QDial* hsDial_;
    QSlider* hillShadeStrength_;
    bool enableHs_;
    bool previousEnableHs_;
    int directionHs_;
    int previousDirectionHs_;
    float strengthHs_;
    float previousStrengthHs_;
    GradientWidget* gradient_;
    //@}
};

}

#endif // __ElevationPanel_h_
