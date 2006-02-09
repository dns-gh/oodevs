// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-12 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/FragmentaryOrderInterface.h $
// $Author: Ape $
// $Modtime: 25/10/04 16:26 $
// $Revision: 2 $
// $Workfile: FragmentaryOrderInterface.h $
//
// *****************************************************************************

#ifndef __FragmentaryOrderInterface_h_
#define __FragmentaryOrderInterface_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MissionInterface_ABC.h"

class ASN_MsgOrderConduite;


// =============================================================================
// Created: APE 2004-05-12
// =============================================================================
class FragmentaryOrderInterface : public MissionInterface_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( FragmentaryOrderInterface );

public:
    //! @name Constructors/Destructor
    //@{
     FragmentaryOrderInterface( Agent_ABC& agent, uint nMissionId, MissionPanel& parentPanel );
    ~FragmentaryOrderInterface();
    //@}

private:
    //! @name Helpers
    //@{
    void CreateInterface();

#include "FragmentaryOrderInterface_Gen.h"
    //@}

private slots:
    void OnOk();

private:
    //! @name Member data
    //@{
    ASN_MsgOrderConduite*   pASNMsgOrder_;
    uint                        nMissionId_;
    //@}
};


#endif // __FragmentaryOrderInterface_h_
