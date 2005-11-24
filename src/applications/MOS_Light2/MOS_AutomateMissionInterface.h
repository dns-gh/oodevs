// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-06 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AutomateMissionInterface.h $
// $Author: Ape $
// $Modtime: 25/10/04 10:45 $
// $Revision: 2 $
// $Workfile: MOS_AutomateMissionInterface.h $
//
// *****************************************************************************

#ifndef __MOS_AutomateMissionInterface_h_
#define __MOS_AutomateMissionInterface_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_MissionInterface_ABC.h"

class MOS_Mission;
class MOS_ASN_MsgAutomateOrder;
class MOS_ParamLimits;

// =============================================================================
// Created: APE 2004-04-20
// =============================================================================
class MOS_AutomateMissionInterface : public MOS_MissionInterface_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_AutomateMissionInterface );

public:
    //! @name Constructors/Destructor
    //@{
    MOS_AutomateMissionInterface( MOS_Agent& agent, uint nMissionId, MOS_MissionPanel& parentPanel );
    ~MOS_AutomateMissionInterface();
    //@}

private:
    //! @name Helpers
    //@{
    void CreateDefaultParameters();
    void CreateInterface();
    //@}

    //! @name Generated helpers
    //@{
    #include "MOS_AutomateMissionInterface_Gen.h" 
    //@}

private slots:
    void OnOk();

private:
    //! @name Member data
    //@{
    uint                      nMissionId_;
    MOS_ASN_MsgAutomateOrder* pASNMsgOrder_;
    MOS_ParamLimits*          pParamLimits_;
    //@}
};

#endif // __MOS_AutomateMissionInterface_h_
