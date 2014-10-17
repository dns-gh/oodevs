// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GradientPreferencesEditor_h_
#define __GradientPreferencesEditor_h_

#include "clients_kernel/OptionsObserver_ABC.h"

namespace kernel
{
    class DetectionMap;
    class OptionsController;
}

namespace gui
{
    class Elevation2dTexture;
    class Gradient;
    class GradientWidget;
    class GradientPreferences;

// =============================================================================
/** @class  GradientPreferencesEditor
    @brief  GradientPreferencesEditor
*/
// Created: SBO 2007-07-02
// =============================================================================
class GradientPreferencesEditor : public QWidget
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             GradientPreferencesEditor( kernel::OptionsController& options,
                                        const kernel::DetectionMap& detection,
                                        const QString& gradientWidgetObjectName,
                                        QWidget* parent = 0 );
    virtual ~GradientPreferencesEditor();
    //@}

    //! @name Operations
    //@{
    void SetElevation2dTexture( const std::shared_ptr< Elevation2dTexture >& texture );
    //@}

signals:
    //! @name Signals
    //@{
    void FitToViewportChanged( int state );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnGradientEdited();
    void OnSelectionChanged();
    void OnPresetCopied();
    void OnPresetRenamed();
    void OnPresetDeleted();
    //@}

private:
    //! @name Helpers
    //@{
    void AddGradient( const std::shared_ptr< Gradient >& gradient );
    void SetCurrent();
    //@}

private:
    //! @name Member data
    //@{
    kernel::OptionsController& options_;
    std::shared_ptr< Elevation2dTexture > elevationTexture_;
    std::shared_ptr< GradientPreferences > preferences_;

    GradientWidget* gradientWidget_;
    QComboBox* combo_;
    //@}
};

} //! namespace gui

#endif // __GradientPreferencesEditor_h_
