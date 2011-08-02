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

#include <Qt3Support/q3vbox.h>

namespace tools
{
    class GeneralConfig;
}

class QCheckBox;
class Q3GroupBox;
class QSpinBox;
class Q3TimeEdit;

// =============================================================================
/** @class  GameConfigPanel
    @brief  GameConfigPanel
*/
// Created: AGE 2007-10-09
// =============================================================================
class GameConfigPanel : public Q3VBox
{
public:
    //! @name Constructors/Destructor
    //@{
             GameConfigPanel( QWidget* parent, const tools::GeneralConfig& config );
    virtual ~GameConfigPanel();
    //@}

    //! @name Operations
    //@{
    // $$$$ AGE 2008-02-22: bouger dans la lib frontend
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
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    QSpinBox* stepSpin_;
    QSpinBox* factorSpin_;
    Q3GroupBox* checkpoints_;
    Q3TimeEdit* checkFrequency_;
    QSpinBox* keepSpin_;
    QCheckBox* decisionalLogs_;
    QCheckBox* pathfindLogs_;
    Q3GroupBox* diaDebugBox_;
    QSpinBox* diaDebugPort_;
    Q3GroupBox* netConBox_;
    QSpinBox* netConPort_;
    QSpinBox* pathThreads_;
    QCheckBox* profile_;
    QCheckBox* checkOdb_;
    //@}
};

#endif // __GameConfigPanel_h_
