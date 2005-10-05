// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MOS_PopulationMissionInterface_h_
#define __MOS_PopulationMissionInterface_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_MissionInterface_ABC.h"

class MOS_Mission;
class MOS_ASN_MsgPionOrder;
class MOS_ASN_MsgPopulationOrder;
class MOS_Population;

// =============================================================================
/** @class  MOS_PopulationMissionInterface
    @brief  MOS_PopulationMissionInterface
    @par    Using example
    @code
    MOS_PopulationMissionInterface;
    @endcode
*/
// Created: APE 2004-04-20
// =============================================================================
class MOS_PopulationMissionInterface : public MOS_MissionInterface_ABC
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_PopulationMissionInterface );

public:
    //! @name Constructors/Destructor
    //@{
    MOS_PopulationMissionInterface( MOS_Population& population, uint nMissionId, MOS_MissionPanel& parentPanel );
    ~MOS_PopulationMissionInterface();
    //@}
   

private:
    //! @name Helpers
    //@{
    void CreateDefaultParameters();
    void CreateInterface();
    //@}


    //! @name Generated helpers
    //@{
     #include "MOS_PopulationMissionInterface_Gen.h" 
    //@}

private slots:
    void OnOk();

private:
    //! @name Member data
    //@{
    uint nMissionId_;
    MOS_ASN_MsgPopulationOrder* pASNMsgOrder_;
    //@}
};


#include "MOS_PopulationMissionInterface.inl"

#endif // __MOS_PopulationMissionInterface_h_
