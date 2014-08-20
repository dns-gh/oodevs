// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __AdvancedConfigPanel_h_
#define __AdvancedConfigPanel_h_

#include "PluginConfig_ABC.h"

namespace tools
{
    class GeneralConfig;
}

class QCheckBox;
class QSpinBox;
class QLabel;

namespace frontend
{
// =============================================================================
/** @class  AdvancedConfigPanel
    @brief  AdvancedConfigPanel
*/
// Created: JSR 2010-07-15
// =============================================================================
class AdvancedConfigPanel : public PluginConfig_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             AdvancedConfigPanel( QWidget* parent, const tools::GeneralConfig& config,
                                  bool hasClient, bool autostartEvents );
    virtual ~AdvancedConfigPanel();
    //@}

    //! @name Operations
    //@{
    virtual void OnLanguageChanged();
    virtual QString GetName() const;
    virtual void Commit( const tools::Path& exercise, const tools::Path& session );
    //@}

signals:
    //! @name Signals
    //@{
    void OnClientEnabled( bool enabled );
    void OnAutostartEvents( bool enabled );
    //@}

private slots:
    //! @name Operations
    //@{
    void OnClientChecked( int state );
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;

    QGroupBox*                  timeBox_;
    QLabel*                     stepLabel_;
    QSpinBox*                   stepSpin_;
    QLabel*                     factorLabel_;
    QSpinBox*                   factorSpin_;
    QLabel*                     endtickLabel_;
    QSpinBox*                   endtickSpin_;
    QLabel*                     pausedLabel_;
    QCheckBox*                  pausedCheckBox_;

    QLabel*                     pathThreadsLabel_;
    QSpinBox*                   pathThreadsSpin_;

    QLabel*                     fragmentsFrequencyLabel_;
    QSpinBox*                   fragmentsFrequencySpin_;

    QLabel*                     noClientLabel_;
    QCheckBox*                  noClientCheckBox_;

    QLabel*                     reportsFrequencyLabel_;
    QSpinBox*                   reportsFrequencySpin_;

    QLabel*                     autostartEventsLabel_;
    QCheckBox*                  autostartEvents_;
    //@}
};
}

#endif // __AdvancedConfigPanel_h_
