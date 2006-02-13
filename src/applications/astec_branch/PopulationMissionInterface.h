// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationMissionInterface_h_
#define __PopulationMissionInterface_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MissionInterface_ABC.h"

class Mission;
class ASN_MsgPionOrder;
class ASN_MsgPopulationOrder;
class Population;

// =============================================================================
// Created: APE 2004-04-20
// =============================================================================
class PopulationMissionInterface : public MissionInterface_ABC
{
    Q_OBJECT
    MT_COPYNOTALLOWED( PopulationMissionInterface );

public:
    //! @name Constructors/Destructor
    //@{
    PopulationMissionInterface( Population& population, uint nMissionId, MissionPanel& parentPanel );
    ~PopulationMissionInterface();
    //@}
   

private:
    //! @name Helpers
    //@{
    void CreateDefaultParameters();
    void CreateInterface();
    //@}


    //! @name Generated helpers
    //@{
     #include "PopulationMissionInterface_Gen.h" 
    //@}

private slots:
    void OnOk();

private:
    //! @name Member data
    //@{
    uint nMissionId_;
    ASN_MsgPopulationOrder* pASNMsgOrder_;
    //@}
};


#endif // __PopulationMissionInterface_h_
