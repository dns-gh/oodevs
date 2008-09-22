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
    class Options;
}

namespace gui
{
    class Elevation2dLayer;
    class Gradient;
    class ColorButton;
    class GradientPreferences;
    class GradientWidget;
    class CheckBox;

// =============================================================================
/** @class  ElevationPanel
    @brief  ElevationPanel
*/
// Created: AGE 2007-01-17
// =============================================================================
class ElevationPanel : public PreferencePanel_ABC
                     , public kernel::Observer_ABC
                     , public kernel::OptionsObserver_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ElevationPanel( QWidget* parent, Elevation2dLayer& layer, kernel::Controllers& controllers );
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
    void OnGradientChanged( const Gradient& gradient );
    void OnEnableVariableGradient( bool );
    void OnEnableHillshade( bool );
    void OnHillShadeDirection( int );
    void OnStrengthChanged( int value );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ElevationPanel( const ElevationPanel& );            //!< Copy constructor
    ElevationPanel& operator=( const ElevationPanel& ); //!< Assignement operator
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

    QGroupBox* hsBox_;
    CheckBox* fitColorGradienttoViewPort_;
    QDial* hsDial_;
    QSlider* hillShadeStrength_;
    bool enableHs_, previousEnableHs_;
    int  directionHs_, previousDirectionHs_;
    float strengthHs_, previousStrengthHs_;
    GradientWidget* gradient_;
    //@}
};

}

#endif // __ElevationPanel_h_
