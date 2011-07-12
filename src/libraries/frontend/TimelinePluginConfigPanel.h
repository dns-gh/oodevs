// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TimelinePluginConfigPanel_h_
#define __TimelinePluginConfigPanel_h_

#include "PluginConfig_ABC.h"

namespace tools
{
    class GeneralConfig;
}

class QLineEdit;
class QGroupBox;
class QTimeEdit;
class QCheckBox;
class QButton;

namespace frontend
{
// =============================================================================
/** @class  TimelinePluginConfigPanel
    @brief  TimelinePluginConfigPanel
*/
// Created: SBO 2008-03-05
// =============================================================================
class TimelinePluginConfigPanel : public PluginConfig_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             TimelinePluginConfigPanel( QWidget* parent, const tools::GeneralConfig& config );
    virtual ~TimelinePluginConfigPanel();
    //@}

    //! @name Operations
    //@{
    virtual QString GetName() const;
    virtual void Commit( const std::string& exercise, const std::string& session );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnOrderClicked();
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    QGroupBox* box_;
    QLineEdit* host_;
    QCheckBox* ssl_;
    QCheckBox* log_;
    QButton*   order_;
    std::string orderFile_;
    QLineEdit* scenarioId_;
    QCheckBox* doRestart_;
    QLineEdit* actorName_;
    QLineEdit* actorId_;
    //@}
};
}

#endif // __TimelinePluginConfigPanel_h_
