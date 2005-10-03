// *****************************************************************************
//
// $Created: AGE 2004-09-16 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/Missions_Skeleton.h $
// $Author: Nld $
// $Modtime: 19/10/04 14:21 $
// $Revision: 2 $
// $Workfile: Missions_Skeleton.h $
//
// *****************************************************************************

#ifndef __Missions_h_
#define __Missions_h_

class MIL_PionMissionType;
class MIL_AutomateMissionType;
class MIL_PopulationMissionType;
class MIL_OrderConduiteType;

namespace missions
{
    enum E_PopulationMission
    {
$PopulationMissionEnumList$
        eNbrPopulationMission
    };


    enum E_AutomateMission
    {
$AutomateMissionEnumList$
        eNbrAutomateMission
    };

    enum E_PionMission
    {
$PionMissionEnumList$
        eNbrPionMission
    };

    enum E_OrderConduite
    {
$OrderConduiteEnumList$
        eNbrOrderConduite
    };

    void RegisterMissions();

    void RegisterAutomateMissions();
    void RegisterPionMissions();
    void RegisterPopulationMissions();
    void RegisterOrderConduites();

} // end namespace missions

#endif // __Missions_h_