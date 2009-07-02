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
             MIL_MissionType_ABC( uint nID, xml::xistream& xis );
    virtual ~MIL_MissionType_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool IsAvailableForModel( const DIA_Model& model ) const = 0;
    virtual const std::string& GetDIABehavior( Phase phase = eNoPhase ) const = 0;
    //@}
    
    //! @name Types
    //@{
    typedef std::map< std::string, const MIL_MissionType_ABC*, sCaseInsensitiveLess > T_MissionNameMap;
    typedef T_MissionNameMap::const_iterator                                          CIT_MissionNameMap;
    //@}
    
protected:
    //! @name Types
    //@{
    typedef std::map< uint, const MIL_MissionType_ABC* > T_MissionIDMap;
    typedef T_MissionIDMap::const_iterator               CIT_MissionIDMap;
    //@}

protected:
    //! @name Helpers
    //@{
    template<typename T> static void ReadMission( xml::xistream& xis, T_MissionIDMap& missionIDs, T_MissionIDMap* pMissionDiaIDs, T_MissionNameMap& missionNames );
    bool IsMissionAvailableForModel( const DIA_Model& model, const std::string& strBehavior ) const;
    //@}
};


// -----------------------------------------------------------------------------
// Name: MIL_MissionType_ABC::ReadMission
// Created: ABL 2007-07-26
// -----------------------------------------------------------------------------
template<typename T>
void MIL_MissionType_ABC::ReadMission( xml::xistream& xis, MIL_MissionType_ABC::T_MissionIDMap& missionIDs, MIL_MissionType_ABC::T_MissionIDMap* pMissionDiaIDs, MIL_MissionType_ABC::T_MissionNameMap& missionNames )
{
    uint nID;
    xis >> xml::attribute( "id", nID );

    const MIL_MissionType_ABC*& pMission = missionIDs[ nID ];
    if( pMission )
        xis.error( "Mission id already defined" );
    pMission = new T( nID, xis );

    if( pMissionDiaIDs )
    {
        const MIL_MissionType_ABC*& pMissionDiaID = (*pMissionDiaIDs)[ pMission->GetDIAType().GetDebugId() ];
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
