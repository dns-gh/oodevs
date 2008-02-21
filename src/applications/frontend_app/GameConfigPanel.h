// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GameConfigPanel_h_
#define __GameConfigPanel_h_

#include <qtabwidget.h>
namespace tools
{
    class GeneralConfig;
}
class QSpinBox;
class QTimeEdit;
class QCheckBox;
class QGroupBox;

// =============================================================================
/** @class  GameConfigPanel
    @brief  GameConfigPanel
*/
// Created: AGE 2007-10-09
// =============================================================================
class GameConfigPanel : public QHBox
{

public:
    //! @name Constructors/Destructor
    //@{
             GameConfigPanel( QWidget* parent, const tools::GeneralConfig& config );
    virtual ~GameConfigPanel();
    //@}

    //! @name Operations
    //@{
    void Commit( const std::string& exercise, const std::string& session, const std::string& name, const std::string& comment, unsigned exerciseNumber );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    GameConfigPanel( const GameConfigPanel& );            //!< Copy constructor
    GameConfigPanel& operator=( const GameConfigPanel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    QWidget* CreateSimulationPanel( QWidget* parent );
    QWidget* CreateDebugPanel( QWidget* parent );
    QWidget* CreateSystemPanel( QWidget* parent );
    std::string GetSessionXml( const std::string& exercise, const std::string& session );
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    QSpinBox* stepSpin_;
    QSpinBox* factorSpin_;
    QTimeEdit* checkFrequency_;
    QSpinBox* keepSpin_;
    QCheckBox* decisionalLogs_;
    QCheckBox* pathfindLogs_;
    QGroupBox* diaDebugBox_;
    QSpinBox* diaDebugPort_;
    QGroupBox* netConBox_;
    QSpinBox* netConPort_;
    QSpinBox* pathThreads_;
    QCheckBox* profile_;
    QCheckBox* checkOdb_;
    //@}
};

#endif // __GameConfigPanel_h_
