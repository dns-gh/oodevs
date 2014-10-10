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
    //! @name Types
    //@{
    typedef std::shared_ptr< Gradient > T_Gradient;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             GradientPreferencesEditor( kernel::OptionsController& options,
                                     const kernel::DetectionMap& detection,
                                     const std::shared_ptr< GradientPreferences >& preferences,
                                     const QString& gradientWidgetObjectName,
                                     QWidget* parent = 0 );
    virtual ~GradientPreferencesEditor();
    //@}

    //! @name Operations
    //@{
    void Load();
    //@}

signals:
    //! @name Signals
    //@{
    void FitToViewportChanged( int state );
    void UpdateGradient();
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
    void AddGradient( const T_Gradient& gradient );
    void SetCurrent();
    //@}

private:
    //! @name Member data
    //@{
    kernel::OptionsController& options_;
    std::shared_ptr< GradientPreferences > preferences_;

    std::map< QString, QString > gradientTranslations_;
    GradientWidget* gradientWidget_;
    QComboBox* combo_;
    //@}
};

} //! namespace gui

#endif // __GradientPreferencesEditor_h_
