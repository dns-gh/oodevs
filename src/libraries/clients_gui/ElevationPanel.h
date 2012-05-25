// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ElevationPanel_h_
#define __ElevationPanel_h_

#include "PreferencePanel_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Gradient;
    class Options;
}

namespace gui
{
    class Elevation2dLayer;
    class ColorButton;
    class GradientPreferences;
    class GradientWidget;
    class CheckBox;
    class Painter_ABC;
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
             ElevationPanel( QWidget* parent, Elevation2dLayer& layer, kernel::Controllers& controllers,
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
    void OnGradientChanged( kernel::Gradient& gradient );
    void OnEnableVariableGradient( bool );
    void OnEnableHillshade( bool );
    void OnHillShadeDirection( int );
    void OnStrengthChanged( int value );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ElevationPanel( const ElevationPanel& );            //!< Copy constructor
    ElevationPanel& operator=( const ElevationPanel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::Options& options_;
    Elevation2dLayer& layer_;
    GradientPreferences& preferences_;
    Q3GroupBox* hsBox_;
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
