// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __SessionConfigPanel_h_
#define __SessionConfigPanel_h_

#include "PluginConfig_ABC.h"

namespace tools
{
    class GeneralConfig;
}

class QLineEdit;
class QSpinBox;
class QLabel;

namespace frontend
{
// =============================================================================
/** @class  SessionConfigPanel
    @brief  SessionConfigPanel
*/
// Created: JSR 2010-11-05
// =============================================================================
class SessionConfigPanel : public PluginConfig_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             SessionConfigPanel( QWidget* parent, const tools::GeneralConfig& config );
    virtual ~SessionConfigPanel();
    //@}

    //! @name Operations
    //@{
    virtual void OnLanguageChanged();
    virtual QString GetName() const;
    virtual void Commit( const tools::Path& exercise, const tools::Path& session );
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    QLineEdit*                  sessionName_;
    QTextEdit*                  sessionComment_;
    QSpinBox*                   exerciseNumber_;
    QLabel*                     nameLabel_;
    QLabel*                     commentLabel_;
    QLabel*                     exerciseLabel_;
    //@}
};
}

#endif // __SessionConfigPanel_h_
