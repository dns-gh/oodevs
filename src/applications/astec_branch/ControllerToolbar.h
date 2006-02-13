// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-09-21 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ControllerToolbar.h $
// $Author: Age $
// $Modtime: 14/04/05 12:53 $
// $Revision: 2 $
// $Workfile: ControllerToolbar.h $
//
// *****************************************************************************

#ifndef __ControllerToolbar_h_
#define __ControllerToolbar_h_

#ifdef __GNUG__
#   pragma interface
#endif

class Team;

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
    QComboBox* pTeamCombo_;
    //@}
};


#endif // __ControllerToolbar_h_
