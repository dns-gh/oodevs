// *****************************************************************************
//
// $Created: $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPopulation_Skeleton.h $
// $Author: Nld $
// $Modtime: 11/03/05 16:34 $
// $Revision: 5 $
// $Workfile: AGR_MissionPopulation_Skeleton.h $
//
// *****************************************************************************

#ifndef __MIL_PopulationMission_FaireMouvement_h_
#define __MIL_PopulationMission_FaireMouvement_h_

#include "simulation_kernel/Entities/Orders/Population/MIL_PopulationMission_ABC.h"
#include "simulation_terrain/TER_Localisation.h"

//*****************************************************************************
// Created: 
//*****************************************************************************
class MIL_PopulationMission_FaireMouvement: public MIL_PopulationMission_ABC
{
    MT_COPYNOTALLOWED( MIL_PopulationMission_FaireMouvement )

public:

             MIL_PopulationMission_FaireMouvement( MIL_Population& population, const MIL_PopulationMissionType& type );
    virtual ~MIL_PopulationMission_FaireMouvement();

    //! @name Init
    //@{
    static void InitializeDIA( const MIL_PopulationMissionType& type );
    /*final*/ ASN1T_EnumOrderErrorCode Initialize( const ASN1T_MsgPopulationOrder& asnMsg );
    /*final*/ //bool                     Initialize( MIL_PopulationMission_ABC& mission );
    /*final*/ void                     Terminate();
    //@}

    //! @name Network 
    //@{
    /*final*/ //void Serialize              ( ASN1T_MsgPopulationOrder& asnMsg );
    /*final*/ //void CleanAfterSerialization( ASN1T_MsgPopulationOrder& asnMsg ); // Delete all the ASN members allocated for the serialization
    //@}

private:
    static int nDIADestinationIdx_;

};


#endif // __MIL_PopulationMission_FaireMouvement_h_
