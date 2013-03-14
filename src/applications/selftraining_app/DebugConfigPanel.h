// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __DebugConfigPanel_h_
#define __DebugConfigPanel_h_

#include "frontend/PluginConfig_ABC.h"

namespace tools
{
    class GeneralConfig;
}

// =============================================================================
/** @class  DebugConfigPanel
    @brief  DebugConfigPanel
*/
// Created: NPT 2013-01-03
// =============================================================================
class DebugConfigPanel : public frontend::PluginConfig_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             DebugConfigPanel( QWidget* parent, const tools::GeneralConfig& config );
    virtual ~DebugConfigPanel();
    //@}

    //! @name Operations
    //@{
    virtual QString GetName() const;
    virtual void Commit( const tools::Path& exercise, const tools::Path& session );
    virtual void OnLanguageChanged();
    //@}

signals:
    //! @name Signals
    //@{
    void SwordVersionSelected( bool isLegacy );
    void IntegrationPathSelected( const tools::Path& integrationPath );
    void DumpPathfindOptionsChanged( const QString& filter, const tools::Path& directory );
    //@}

private:
    //! @name Member data
    //@{
    //config
    const tools::GeneralConfig& config_;

    QStringList pathList_; // $$$$ ABR 2013-03-04: TODO Extract this to a new ComboBox class which will handle a registry based historic
    //legacy
    QCheckBox* legacyCheckBox_;
    QGroupBox* legacyBox_;

    //Integration Layer Configuration
    QLabel* integrationLabel_;
    QComboBox* integrationComboBox_;
    QPushButton* integrationButton_;

    //profiling configuration
    QGroupBox* profilingBox_;
    QCheckBox* decCallsBox_;
    QCheckBox* commandsBox_;
    QCheckBox* hooksBox_;

    //pathfinds configuration
    QGroupBox* pathfindsBox_;
    QLabel* dumpLabel_;
    QLineEdit* dataDirectory_;
    QPushButton* dataButton_;
    QLabel* filterLabel_;
    QLineEdit* filterEdit_;
    //@}

private slots:
    //! @name Operations
    //@{
    void SwordVersionChecked( bool state );
    void OnChangeIntegrationDirectory();
    void OnEditIntegrationDirectory( const QString& );
    void OnChangeDataDirectory();
    void OnChangeDataFilter();
    //@}
};

#endif // __DebugConfigPanel_h_
