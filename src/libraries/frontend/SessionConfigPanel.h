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
class QTextEdit;

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
public:
    //! @name Constructors/Destructor
    //@{
             SessionConfigPanel( QWidget* parent, const tools::GeneralConfig& config );
    virtual ~SessionConfigPanel();
    //@}

    //! @name Operations
    //@{
    virtual void Commit( const std::string& exercise, const std::string& session );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SessionConfigPanel( const SessionConfigPanel& );            //!< Copy constructor
    SessionConfigPanel& operator=( const SessionConfigPanel& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    QLineEdit* sessionName_;
    QTextEdit* sessionComment_;
    QSpinBox* exerciseNumber_;
    //@}
};

}

#endif // __SessionConfigPanel_h_
