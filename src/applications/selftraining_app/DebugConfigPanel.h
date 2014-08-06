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

#include "clients_gui/WidgetLanguageObserver_ABC.h"
#include <boost/noncopyable.hpp>

namespace frontend
{
    struct DebugConfig;
}

namespace tools
{
    class GeneralConfig;
}

class Config;

// =============================================================================
/** @class  DebugConfigPanel
    @brief  DebugConfigPanel
*/
// Created: NPT 2013-01-03
// =============================================================================
class DebugConfigPanel : public gui::WidgetLanguageObserver_ABC< QWidget >
                       , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             DebugConfigPanel( QWidget* parent, const Config& config, frontend::DebugConfig& debug );
    virtual ~DebugConfigPanel();
    //@}

    //! @name Operations
    //@{
    QString GetName() const;
    virtual void OnLanguageChanged();
    //@}

private slots:
    //! @name Operations
    //@{
    void OnTimelineChecked( bool checked );
    void OnTimelineDebugPortChanged( int port );
    void OnTimelineLogChanged( const QString& );
    void OnTimelineDebugChanged( const QString& );
    void OnCefLogChanged( const QString& );
    void OnMapnikLayerChecked( bool checked );
    void OnMapnikThreadsChanged( int threads );
    void OnChangeIntegrationDirectory();
    void OnEditIntegrationDirectory( const QString& );
    void OnSelectDataDirectory();
    void OnChangeDataDirectory();
    void OnDevFeaturesChanged();
    void OnDecProfilingChanged( bool checked );
    //@}

private:
    //! @name Member data
    //@{
    //config
    const bool visible_;
    const Config& config_;
    frontend::DebugConfig& debug_;

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

    //Integration Layer Configuration
    QLabel* integrationLabel_;
    QLineEdit* integrationDir_;
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
    QSpinBox* mapnikThreads_;
    QLabel* mapnikThreadsLabel_;

    // development features
    QGroupBox* featuresBox_;
    std::vector< QCheckBox* > features_;
    //@}
};

#endif // __DebugConfigPanel_h_
