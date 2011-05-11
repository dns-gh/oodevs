// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_MissionType_ABC_h_
#define __MIL_MissionType_ABC_h_

#include "MIL_OrderType_ABC.h"
#include "MT_Tools/MT_String.h"

// =============================================================================
/** @class  MIL_MissionType_ABC
    @brief  MIL_MissionType_ABC
*/
// Created: NLD 2006-11-14
// =============================================================================
class MIL_MissionType_ABC : public MIL_OrderType_ABC
{
public:
    //! @name Phase enum to access the behavior string
    //@{
    enum Phase
    {
        eNoPhase = 0,
        ePhaseMRT,
        ePhaseCDT
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             MIL_MissionType_ABC( unsigned int nID, unsigned int contextLength, xml::xistream& xis );
    virtual ~MIL_MissionType_ABC();
    //@}

    //! @name Operations
    //@{
    virtual const std::string& GetDIABehavior( Phase phase = eNoPhase ) const = 0;
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, const MIL_MissionType_ABC* > T_MissionNameMap;
    typedef T_MissionNameMap::const_iterator CIT_MissionNameMap;
    //@}

protected:
    MIL_MissionType_ABC();

    //! @name Types
    //@{
    typedef std::map< unsigned int, const MIL_MissionType_ABC* > T_MissionIDMap;
    typedef T_MissionIDMap::const_iterator                     CIT_MissionIDMap;
    //@}

protected:
    //! @name Helpers
    //@{
    template<typename T> static void ReadMission( xml::xistream& xis, T_MissionIDMap& missionIDs, T_MissionNameMap* pMissionDiaIDs, T_MissionNameMap& missionNames );
    //@}
};

// -----------------------------------------------------------------------------
// Name: MIL_MissionType_ABC::ReadMission
// Created: ABL 2007-07-26
// -----------------------------------------------------------------------------
template<typename T>
void MIL_MissionType_ABC::ReadMission( xml::xistream& xis, MIL_MissionType_ABC::T_MissionIDMap& missionIDs, MIL_MissionType_ABC::T_MissionNameMap* pMissionDiaIDs, MIL_MissionType_ABC::T_MissionNameMap& missionNames )
{
    unsigned int nID;
    xis >> xml::attribute( "id", nID );

    const MIL_MissionType_ABC*& pMission = missionIDs[ nID ];
    if( pMission )
        xis.error( "Mission id already defined" );
    pMission = new T( nID, xis );

    if( pMissionDiaIDs )
    {
        const MIL_MissionType_ABC*& pMissionDiaID = (*pMissionDiaIDs)[ pMission->GetDIAType() ];
        if( pMissionDiaID )
            xis.error( "Mission DIA ID already defined" );
        pMissionDiaID = pMission;
    }

    const MIL_MissionType_ABC*& pMissionName = missionNames[ pMission->GetName() ];
    if( pMissionName )
        xis.error( "Mission name already defined" );
    pMissionName = pMission;
}

#endif // __MIL_MissionType_ABC_h_
