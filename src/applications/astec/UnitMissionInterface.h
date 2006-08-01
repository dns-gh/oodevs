// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-04-20 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/UnitMissionInterface.h $
// $Author: Ape $
// $Modtime: 25/10/04 10:14 $
// $Revision: 3 $
// $Workfile: UnitMissionInterface.h $
//
// *****************************************************************************

#ifndef __UnitMissionInterface_h_
#define __UnitMissionInterface_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MissionInterface_ABC.h"

class Mission;
class ASN_MsgPionOrder;
class Agent;
class OptionalParamFunctor_ABC;

// =============================================================================
// Created: APE 2004-04-20
// =============================================================================
class UnitMissionInterface : public MissionInterface_ABC
{
    Q_OBJECT
    MT_COPYNOTALLOWED( UnitMissionInterface );

public:
    //! @name Constructors/Destructor
    //@{
    UnitMissionInterface( Agent& agent, uint nMissionId, MissionPanel& parentPanel );
    ~UnitMissionInterface();
    //@}

private:
    //! @name Helpers
    //@{
    void CreateDefaultParameters();
    void CreateInterface();
    //@}


    //! @name Generated helpers
    //@{
#include "UnitMissionInterface_Gen.h" 
    //@}

private slots:
    void OnOk();

private:
    //! @name Member data
    //@{
    uint nMissionId_;
    ASN_MsgPionOrder*               pASNMsgOrder_;
    //@}
};

#endif // __UnitMissionInterface_h_
