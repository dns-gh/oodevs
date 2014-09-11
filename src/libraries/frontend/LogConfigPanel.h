    // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __LogConfigPanel_h_
#define __LogConfigPanel_h_

#include "PluginConfig_ABC.h"

namespace tools
{
    class GeneralConfig;
}

class QComboBox;
class QCheckBox;
class QSpinBox;
class QLabel;
class QPushButton;

namespace frontend
{
    class CreateSession;

// =============================================================================
/** @class  LogConfigPanel
    @brief  LogConfigPanel
*/
// Created: JSR 2014-09-11
// =============================================================================
class LogConfigPanel : public PluginConfig_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             LogConfigPanel( QWidget* parent, const tools::GeneralConfig& config );
    virtual ~LogConfigPanel();
    //@}

    //! @name Operations
    //@{
    virtual void OnLanguageChanged();
    virtual QString GetName() const;
    virtual void Commit( const tools::Path& exercise, const tools::Path& session );
    //@}

private:
    //! @name Helpers
    //@{
    void Commit( frontend::CreateSession& action, const std::string& path ) const;
    //@}

private slots:
    void OnKeepToggled( bool checked );
    void OnDefaultClicked();
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    QGroupBox* rotateLogsGroup_;
    QLabel* fileSizeLabel_;
    QSpinBox* fileSizeSpinBox_;
    QComboBox* sizeTypeComboBox_;
    QCheckBox* keepCheckBox_;
    QSpinBox* keepSpinBox_;
    QLabel* keepLabel_;
    QLabel* levelLabel_;
    QComboBox* levelComboBox_;
    QPushButton* defaultButton_;
    //@}
};
}

#endif // __LogConfigPanel_h_
