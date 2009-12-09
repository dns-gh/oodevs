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
#include "Functions/DEC_FrontAndBackLinesComputer.h"
#include "Decision/DEC_Gen_Object.h"
#include "Decision/DEC_Objective.h"

namespace directia
{
// $$$$ LDC: TODO Check which kind of deletion/destruction operations must be done. See ManageDeletion below...
    void UsedByDIA( DEC_Decision_ABC* ) {}
    void ReleasedByDIA( DEC_Decision_ABC* ) {}
    void UsedByDIA( DEC_FrontAndBackLinesComputer* ) {}
    void ReleasedByDIA( DEC_FrontAndBackLinesComputer* ) {}
    void UsedByDIA( DEC_Gen_Object* ) {}
    void ReleasedByDIA( DEC_Gen_Object* ) {}
    void UsedByDIA( DEC_Knowledge_Agent* ) {}
    void ReleasedByDIA( DEC_Knowledge_Agent* ) {}
    void UsedByDIA( DEC_Knowledge_Object* ) {}
    void ReleasedByDIA( DEC_Knowledge_Object* ) {}
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
    void UsedByDIA( urban::Block* ) {}
    void ReleasedByDIA( urban::Block* ) {}
}

// -----------------------------------------------------------------------------
// Name: DEC_Tools::ManageDeletion
// Created: NLD 2005-12-09
// -----------------------------------------------------------------------------
//void DEC_Tools::ManageDeletion( void* pPtr, const DIA_Type* pType )
//{
//    assert( pType );
//    if( !pPtr )
//        return;
// $$$$ LDC: TODO Are the delete and other operations below still worth doing in DIA4?
//    if( *pType == *pTypePoint_ )
//        delete static_cast< MT_Vector2D* >( pPtr );
//    else if( *pType == *pTypeDirection_ )
//        delete static_cast< MT_Vector2D* >( pPtr );
//    else if( *pType == *pTypeItineraire_ )
//    {
//        DEC_Agent_Path* pPath = static_cast< DEC_Agent_Path* >( pPtr );
//        pPath->Cancel();
//        pPath->DecDIARef();
//    }
//    else if( *pType == *pTypeListePoints_ )
//        delete static_cast< T_PointVector* >( pPtr );
//    else if( *pType == *pTypeLocalisation_ )
//        delete static_cast< TER_Localisation* >( pPtr );
//    else if( *pType == *pTypeCalculLignesAvantArriere_ )
//        delete static_cast< DEC_FrontAndBackLinesComputer* >( pPtr );
//    else if( *pType == *pTypeGenObjet_ )
//        delete static_cast< DEC_Gen_Object* >( pPtr );
//    else if( *pType == *pTypeMaintenancePriorities_ )
//        delete static_cast< T_MaintenancePriorityVector* >( pPtr );
//    else if( *pType == *pTypeSantePriorites_ )
//        delete static_cast< T_MedicalPriorityVector* >( pPtr );
//    else if( *pType == *pTypeFuseau_ )
//        delete static_cast< MIL_Fuseau* >( pPtr );
//    else if( *pType == *pTypeObjectif_ )
//        delete static_cast< DEC_Objective* >( pPtr );
//    else
//        assert( false );
//}
