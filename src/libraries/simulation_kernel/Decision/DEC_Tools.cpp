// *****************************************************************************
//
// $Created: NLD 2004-10-20 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/DEC_Tools.cpp $
// $Author: Jvt $
// $Modtime: 11/05/05 17:04 $
// $Revision: 17 $
// $Workfile: DEC_Tools.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Tools.h"
#include "Decision/DEC_FrontAndBackLinesComputer.h"
#include "Decision/DEC_Gen_Object.h"
#include "Decision/DEC_Objective.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"

namespace directia
{
// $$$$ LDC: TODO Check which kind of deletion/destruction operations must be done. See ManageDeletion below...
    void UsedByDIA( DEC_Decision_ABC* dec )
    {
        dec->IncDIARef();
    }
    void ReleasedByDIA( DEC_Decision_ABC* dec )
    {
        dec->DecDIARef();
    }
    void UsedByDIA( DEC_FrontAndBackLinesComputer* ) {}
    void ReleasedByDIA( DEC_FrontAndBackLinesComputer* ) {}
    void UsedByDIA( DEC_Gen_Object* ) {}
    void ReleasedByDIA( DEC_Gen_Object* ) {}
    void UsedByDIA( DEC_Knowledge_Agent* ) {}
    void ReleasedByDIA( DEC_Knowledge_Agent* ) {}
    void UsedByDIA( DEC_Knowledge_Object* ) {}
    void ReleasedByDIA( DEC_Knowledge_Object* ) {}
    void UsedByDIA( MIL_UrbanObject_ABC* ) {}
    void ReleasedByDIA( MIL_UrbanObject_ABC* ) {}
    void UsedByDIA( DEC_Knowledge_Population* ) {}
    void ReleasedByDIA( DEC_Knowledge_Population* ) {}
    void UsedByDIA( DEC_Objective* ) {}
    void ReleasedByDIA( DEC_Objective* ) {}
    void UsedByDIA( DEC_Path_ABC* ) {}
    void ReleasedByDIA( DEC_Path_ABC* ) {}
    void UsedByDIA( DEC_PathPoint* ) {}
    void ReleasedByDIA( DEC_PathPoint* ) {}
    void UsedByDIA( MIL_FragOrder* ) {}
    void ReleasedByDIA( MIL_FragOrder* ) {}
    void UsedByDIA( MIL_Fuseau* ) {}
    void ReleasedByDIA( MIL_Fuseau* ) {}
    void UsedByDIA( MIL_LimaOrder* ) {}
    void ReleasedByDIA( MIL_LimaOrder* ) {}
    void UsedByDIA( MIL_ParameterType_MedicalPriorities* ) {}
    void ReleasedByDIA( MIL_ParameterType_MedicalPriorities* ) {}
    void UsedByDIA( MIL_ParameterType_MaintenancePriorities* ) {}
    void ReleasedByDIA( MIL_ParameterType_MaintenancePriorities* ) {}
    void UsedByDIA( MT_Vector2D* ) {}
    void ReleasedByDIA( MT_Vector2D* ) {}
    void UsedByDIA( PHY_DotationCategory* ) {}
    void ReleasedByDIA( PHY_DotationCategory* ) {}
    void UsedByDIA( PHY_ComposanteTypePion* ) {}
    void ReleasedByDIA( PHY_ComposanteTypePion* ) {}
    void UsedByDIA( PHY_HumanWound* ) {}
    void ReleasedByDIA( PHY_HumanWound* ) {}
    void UsedByDIA( TER_Localisation* ) {}
    void ReleasedByDIA( TER_Localisation* ) {}
    void UsedByDIA( PHY_Action_ABC* ) {}
    void ReleasedByDIA( PHY_Action_ABC* ) {}
    void UsedByDIA( PHY_ResourceNetworkType* ) {}
    void ReleasedByDIA( PHY_ResourceNetworkType* ) {}
}
