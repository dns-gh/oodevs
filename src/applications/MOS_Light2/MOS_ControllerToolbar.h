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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ControllerToolbar.h $
// $Author: Age $
// $Modtime: 14/04/05 12:53 $
// $Revision: 2 $
// $Workfile: MOS_ControllerToolbar.h $
//
// *****************************************************************************

#ifndef __MOS_ControllerToolbar_h_
#define __MOS_ControllerToolbar_h_

#ifdef __GNUG__
#   pragma interface
#endif

class MOS_Team;

// =============================================================================
/** @class  MOS_ControllerToolbar
    @brief  Toolbar for admin tools (changing team, launching the sim...) 
*/
// Created: APE 2004-09-21
// =============================================================================
class MOS_ControllerToolbar : public QToolBar
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_ControllerToolbar );

public:
    //! @name Constructors/Destructor
    //@{
     MOS_ControllerToolbar( QMainWindow* pParent );
    ~MOS_ControllerToolbar();
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
    void OnTeamCreated( MOS_Team& team );
    void LaunchScipio();
    void LaunchScipioDbg();
    //@}

private:
    //! @name Member data
    //@{
    QComboBox* pTeamCombo_;
    //@}
};


#include "MOS_ControllerToolbar.inl"

#endif // __MOS_ControllerToolbar_h_
