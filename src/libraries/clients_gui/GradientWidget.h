// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GradientWidget_h_
#define __GradientWidget_h_

#include "clients_kernel/OptionsObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class OptionsController;
}

namespace gui
{
    class ColorButton;
    class Gradient;
    class GradientButton;
    class GradientPreferences;
    class GradientsCombo;
    class Painter_ABC;
    template< typename T > class RichWidget;
}

namespace gui
{
// =============================================================================
/** @class  GradientWidget
    @brief  GradientWidget
*/
// Created: SBO 2007-07-02
// =============================================================================
class GradientWidget : public Q3VBox
                     , public tools::Observer_ABC
                     , public kernel::OptionsObserver_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             GradientWidget( QWidget* parent, GradientPreferences& preferences, kernel::Controllers& controllers,
                             const Painter_ABC& painter );
    virtual ~GradientWidget();
    //@}

    //! @name Operations
    //@{
    void Commit();
    void Reset();
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

signals:
    //! @name Signals
    //@{
    void GradientChanged( Gradient& );
    void ToggleVariableGradient( bool state );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChanged( const QColor& color );
    void OnEnableVariableGradient( bool state );
    void OnColorChanged( const QColor& color );
    void OnGradientEdited( Gradient& gradient );

    void OnPresetChanged();
    void OnPresetCopied();
    void OnPresetRenamed();
    void OnPresetDeleted();
    //@}

private:
    //! @name Helpers
    //@{
    Gradient* CurrentPreset() const;
    void Clear();
    void Select( const QString& presetName );
    //@}

    //! @name Types
    //@{
    typedef std::vector< Gradient* > T_Gradients;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::OptionsController& options_;
    GradientPreferences& preferences_;
    GradientButton* gradientEditor_;
    ColorButton* color_;
    RichWidget< QComboBox >* presetCombo_;
    T_Gradients presets_;
    //@}
};

}

#endif // __GradientWidget_h_
