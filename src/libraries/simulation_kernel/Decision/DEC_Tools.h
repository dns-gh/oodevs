// *****************************************************************************
//
// $Created: NLD 2004-08-19 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/decision/DEC_Tools.h $
// $Author: Jvt $
// $Modtime: 11/05/05 17:03 $
// $Revision: 22 $
// $Workfile: DEC_Tools.h $
//
// *****************************************************************************

#ifndef __DEC_Tools_h_
#define __DEC_Tools_h_

class DEC_Decision_ABC;
class DEC_FrontAndBackLinesComputer;
class DEC_Gen_Object;
class DEC_Knowledge_Agent;
class DEC_Knowledge_Object;
class DEC_Knowledge_Population;
class DEC_Objective;
class DEC_Path_ABC;
class DEC_PathPoint;
class MIL_FragOrder;
class MIL_Fuseau;
class MIL_LimaOrder;
class MIL_ParameterType_MedicalPriorities;
class MIL_ParameterType_MaintenancePriorities;
class MT_Vector2D;
class PHY_Action_ABC;
class PHY_DotationCategory;
class PHY_ComposanteTypePion;
class PHY_HumanWound;
class PHY_Population_ActionFireOnPion;
class PHY_Population_ActionFireOnPions;
class PHY_Population_ActionMove;
class PHY_ResourceNetworkType;
class TER_Localisation;
class MIL_UrbanObject_ABC;

namespace directia
{
    void UsedByDIA( DEC_Decision_ABC* );
    void ReleasedByDIA( DEC_Decision_ABC* );
    void UsedByDIA( DEC_FrontAndBackLinesComputer* );
    void ReleasedByDIA( DEC_FrontAndBackLinesComputer* );
    void UsedByDIA( DEC_Gen_Object* );
    void ReleasedByDIA( DEC_Gen_Object* );
    void UsedByDIA( DEC_Knowledge_Agent* );
    void ReleasedByDIA( DEC_Knowledge_Agent* );
    void UsedByDIA( DEC_Knowledge_Object* );
    void ReleasedByDIA( DEC_Knowledge_Object* );
    void UsedByDIA( MIL_UrbanObject_ABC* );
    void ReleasedByDIA( MIL_UrbanObject_ABC* );
    void UsedByDIA( DEC_Knowledge_Population* );
    void ReleasedByDIA( DEC_Knowledge_Population* );
    void UsedByDIA( DEC_Objective* );
    void ReleasedByDIA( DEC_Objective* );
    void UsedByDIA( DEC_Path_ABC* );
    void ReleasedByDIA( DEC_Path_ABC* );
    void UsedByDIA( DEC_PathPoint* );
    void ReleasedByDIA( DEC_PathPoint* );
    void UsedByDIA( MIL_FragOrder* );
    void ReleasedByDIA( MIL_FragOrder* );
    void UsedByDIA( MIL_Fuseau* );
    void ReleasedByDIA( MIL_Fuseau* );
    void UsedByDIA( MIL_LimaOrder* );
    void ReleasedByDIA( MIL_LimaOrder* );
    void UsedByDIA( MIL_ParameterType_MedicalPriorities* );
    void ReleasedByDIA( MIL_ParameterType_MedicalPriorities* );
    void UsedByDIA( MIL_ParameterType_MaintenancePriorities* );
    void ReleasedByDIA( MIL_ParameterType_MaintenancePriorities* );
    void UsedByDIA( MT_Vector2D* );
    void ReleasedByDIA( MT_Vector2D* );
    void UsedByDIA( PHY_DotationCategory* );
    void ReleasedByDIA( PHY_DotationCategory* );
    void UsedByDIA( PHY_ComposanteTypePion* );
    void ReleasedByDIA( PHY_ComposanteTypePion* );
    void UsedByDIA( PHY_HumanWound* );
    void ReleasedByDIA( PHY_HumanWound* );
    void UsedByDIA( TER_Localisation* );
    void ReleasedByDIA( TER_Localisation* );
    void UsedByDIA( PHY_Action_ABC* );
    void ReleasedByDIA( PHY_Action_ABC* );
    void UsedByDIA( PHY_ResourceNetworkType* );
    void ReleasedByDIA( PHY_ResourceNetworkType* );
}

#endif // __DEC_Tools_h_
