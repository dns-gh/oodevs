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

#include "MIL.h"

#include "MIL_AgentServer.h"
#include "Tools/MIL_ProfilerMgr.h"

namespace urban
{
    class Block;
}

class DIA_FunctionCall;
class DIA_Instance;
class DIA_Instruction_ABC;
class DIA_Type;
class DIA_TypeDef;
class DIA_TypedObject;

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
class TER_Localisation;

namespace directia
{
    void UsedByDIA( DIA_TypedObject* );
    void ReleasedByDIA( DIA_TypedObject* );
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
    void UsedByDIA( urban::Block* );
    void ReleasedByDIA( urban::Block* );
}

// =============================================================================
// @class  DEC_Tools
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Tools
{
public:
    //! @name Tools
    //@{
    static const DIA_TypeDef& GetDIAType        ( const std::string& strTypeName );
    static int                InitializeDIAField( const std::string& strFieldName, const DIA_TypeDef& diaType );
    static void               ManageDeletion    ( void* pPtr, const DIA_Type* pType );
    static void               DisplayDiaStack   ( const DIA_Instance* pInstance, const DIA_Instruction_ABC* pInstruction );
    //@}
};

//! @name DIA Functions registration tools
//@{
template< class FunctionCall, class CallerType >
struct DEC_DIACallFunctor
{
    FunctionCall function_;

    DEC_DIACallFunctor( FunctionCall function ) : function_( function ) {}

    void operator()( DIA_FunctionCall& parameters ) const
    {
        CallerType& executer = static_cast< DIA_FunctionCaller< CallerType >* >( parameters.GetFunctionCaller() )->GetCalledObject();
        (*function_)( parameters, executer );
    }
};

template< class FunctionCall, class CallerType >
struct DEC_DIACallFunctorWithProfiling
{
    FunctionCall function_;

    DEC_DIACallFunctorWithProfiling( FunctionCall function ) : function_( function ) {}

    void operator()( DIA_FunctionCall& parameters ) const
    {
        CallerType& executer = static_cast< DIA_FunctionCaller< CallerType >* >( parameters.GetFunctionCaller() )->GetCalledObject();

        MT_Profiler profiler;
        profiler.Start();
        (*function_)( parameters, executer );
        MIL_AgentServer::GetWorkspace().GetProfilerManager().NotifyDecFunctionCalled( executer, parameters.GetName(), profiler.Stop() );
    }
};
//@}

#include "DEC_Tools.inl"


#endif // __DEC_Tools_h_
