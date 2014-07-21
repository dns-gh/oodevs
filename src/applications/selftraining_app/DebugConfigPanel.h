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

// Returns an absolute path to timeline client log file. logPath is resolved
// relatively to sessionDir. If logPath is empty, an empty path is returned.
tools::Path GetTimelineLog( const tools::Path& sessionDir, const tools::Path& logPath );

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
    virtual bool IsVisible() const;
    virtual void Commit( const tools::Path& exercise, const tools::Path& session );
    virtual void OnLanguageChanged();
    //@}

    //! @name Accessors
    //@{
    QString GetDevFeatures() const;
    //@}

signals:
    //! @name Signals
    //@{
    void IntegrationPathSelected( const tools::Path& integrationPath );
    void DumpPathfindOptionsChanged( const QString& filter, const tools::Path& directory );
    //@}

private slots:
    //! @name Operations
    //@{
    void OnTimelineChecked( bool checked );
    void OnTimelineDebugPortChanged( int port );
    void OnTimelineLogChanged( const QString& );
    void OnTimelineDebugChanged( const QString& );
    void OnCefLogChanged( const QString& );
    void OnExerciseNumberChanged( int exerciseNumber );
    void OnMapnikLayerChecked( bool checked );
    void OnChangeIntegrationDirectory();
    void OnEditIntegrationDirectory( const QString& );
    void OnSelectDataDirectory();
    void OnChangeDataDirectory();
    void OnChangeDataFilter();
    void OnDevFeaturesChanged();
    //@}

private:
    //! @name Member data
    //@{
    //config
    const bool visible_;
    const tools::GeneralConfig& config_;

    QStringList pathList_; // $$$$ ABR 2013-03-04: TODO Extract this to a new ComboBox class which will handle a registry based history

    QGroupBox* topBox_;

    // timeline
    QGroupBox* timelineBox_;
    QLabel* timelineDebugPortLabel_;
    QSpinBox* timelineDebugPortSpinBox_;
    QLabel* timelineLogLabel_;
    QLineEdit* timelineLog_;
    QLabel* timelineDebugLabel_;
    QLineEdit* timelineDebug_;
    QLabel* cefLogLabel_;
    QLineEdit* cefLog_;
    QCheckBox* oldTimeline_;
    int exerciseNumber_;

    //Integration Layer Configuration
    QLabel* integrationLabel_;
    QComboBox* integrationComboBox_;
    QPushButton* integrationButton_;

    //profiling configuration
    QGroupBox* profilingBox_;
    QCheckBox* decCallsBox_;

    //pathfinds configuration
    QGroupBox* pathfindsBox_;
    QLabel* dumpLabel_;
    QLineEdit* dataDirectory_;
    QPushButton* dataButton_;
    QLabel* filterLabel_;
    QLineEdit* filterEdit_;

    // mapnik
    QGroupBox* mapnikBox_;
    QCheckBox* mapnikLayerBox_;

    // development features
    QGroupBox* featuresBox_;
    std::vector< QCheckBox* > features_;
    //@}
};

#endif // __DebugConfigPanel_h_
