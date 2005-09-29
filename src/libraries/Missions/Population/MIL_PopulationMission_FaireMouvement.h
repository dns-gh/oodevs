// *****************************************************************************
//
// $Created: 2005-08-19 - 11:30:56 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.h $
// $Author: Nld $
// $Modtime: 11/03/05 16:34 $
// $Revision: 5 $
// $Workfile: AGR_MissionPion_Skeleton.h $
//
// *****************************************************************************

#ifndef __MIL_PopulationMission_FaireMouvement_h_
#define __MIL_PopulationMission_FaireMouvement_h_

#include "MIL/Entities/Orders/Population/MIL_PopulationMission_ABC.h"

//*****************************************************************************
// Created: 2005-08-19 - 11:30:56
//*****************************************************************************
class MIL_PopulationMission_FaireMouvement : public MIL_PopulationMission_ABC
{
    MT_COPYNOTALLOWED( MIL_PopulationMission_FaireMouvement )

public:

             MIL_PopulationMission_FaireMouvement( MIL_Population& population, const MIL_PopulationMissionType& type );
    virtual ~MIL_PopulationMission_FaireMouvement();

    //! @name Init
    //@{
    static void InitializeDIA( const MIL_PopulationMissionType& type );
    /*final*/ ASN1T_EnumOrderErrorCode Initialize( const ASN1T_MsgPopulationOrder& asnMsg );
    /*final*/ void                     Terminate();
    //@}

private:
    static int nDIADestinationIdx_;

private:
    MT_Vector2D destination_;

};


#endif // __MIL_PopulationMission_FaireMouvement_h_
