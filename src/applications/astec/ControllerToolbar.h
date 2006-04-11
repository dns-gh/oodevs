// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ControllerToolbar_h_
#define __ControllerToolbar_h_

class Team;
class Options;

// =============================================================================
/** @class  ControllerToolbar
    @brief  Toolbar for admin tools (changing team, launching the sim...) 
*/
// Created: APE 2004-09-21
// =============================================================================
class ControllerToolbar : public QToolBar
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( ControllerToolbar );

public:
    //! @name Constructors/Destructor
    //@{
     ControllerToolbar( QMainWindow* pParent );
    ~ControllerToolbar();
    //@}

signals:
    //! @name Signals
    //@{
    void TeamChanged();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnTeamChanged( int nValue );
    void OnTeamDeleted( Team& team );
    void OnTeamCreated( Team& team );
    void LaunchScipio();
    void LaunchScipioDbg();
    //@}

private:
    //! @name Member data
    //@{
    Options& options_;
    QComboBox* pTeamCombo_;
    //@}
};


#endif // __ControllerToolbar_h_
