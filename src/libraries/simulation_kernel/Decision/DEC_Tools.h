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

class DIA_Instance;
class DIA_Instruction_ABC;

// =============================================================================
// @class  DEC_Tools
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Tools
{
public:
    //! @name Tools
    //@{
    static void               InitializeDIA     ();
    static const DIA_TypeDef& GetDIAType        ( const std::string& strTypeName );
    static int                InitializeDIAField( const std::string& strFieldName, const DIA_TypeDef& diaType );
    static void               ManageDeletion    ( void* pPtr, const DIA_Type* pType );
    static void               DisplayDiaStack   ( const DIA_Instance* pInstance, const DIA_Instruction_ABC* pInstruction );
    //@}

    //! @name Checkers
    //@{
    static bool CheckTypePoint                              ( const DIA_Variable_ABC& diaVariable );
    static bool CheckTypeDirection                          ( const DIA_Variable_ABC& diaVariable );
    static bool CheckTypeListeDirection                     ( const DIA_Variable_ABC& diaVariable );
    static bool CheckTypeItineraire                         ( const DIA_Variable_ABC& diaVariable );
    static bool CheckTypeListePoints                        ( const DIA_Variable_ABC& diaVariable );
    static bool CheckTypeListeListesPoints                  ( const DIA_Variable_ABC& diaVariable );
    static bool CheckTypeLocalisation                       ( const DIA_Variable_ABC& diaVariable );
    static bool CheckTypeListeLocalisations                 ( const DIA_Variable_ABC& diaVariable );
    static bool CheckTypeCalculLignesAvantArriere           ( const DIA_Variable_ABC& diaVariable );
    static bool CheckTypeLima                               ( const DIA_Variable_ABC& diaVariable );
    static bool CheckTypeID                                 ( const DIA_Variable_ABC& diaVariable );
    static bool CheckTypeAction                             ( const DIA_Variable_ABC& diaVariable );
    static bool CheckTypePion                               ( const DIA_Variable_ABC& diaVariable );
    static bool CheckTypePion                               ( const DIA_TypedObject&  object      );
    static bool CheckTypeListePions                         ( const DIA_Variable_ABC& diaVariable );
    static bool CheckTypeAutomate                           ( const DIA_Variable_ABC& diaVariable );
    static bool CheckTypeAutomate                           ( const DIA_TypedObject&  object      );
    static bool CheckTypeListeAutomates                     ( const DIA_Variable_ABC& diaVariable );
    static bool CheckTypeGenObjet                           ( const DIA_Variable_ABC& diaVariable );
    static bool CheckTypeMissionPion                        ( const DIA_Variable_ABC& diaVariable );
    static bool CheckTypeConnaissanceAgent                  ( const DIA_Variable_ABC& diaVariable );
    static bool CheckTypeListeConnaissancesAgent            ( const DIA_Variable_ABC& diaVariable );
    static bool CheckTypeConnaissanceObjet                  ( const DIA_Variable_ABC& diaVariable );
    static bool CheckTypeListeConnaissancesObjet            ( const DIA_Variable_ABC& diaVariable );
    static bool CheckTypeConnaissancePopulation             ( const DIA_Variable_ABC& diaVariable );
    static bool CheckTypeListeConnaissancesPopulation       ( const DIA_Variable_ABC& diaVariable );
    static bool CheckTypePopulationConnaissanceAgent        ( const DIA_Variable_ABC& diaVariable );
    static bool CheckTypeListePopulationConnaissancesAgent  ( const DIA_Variable_ABC& diaVariable );
    static bool CheckTypePopulationConnaissanceObjet        ( const DIA_Variable_ABC& diaVariable );
    static bool CheckTypeListePopulationConnaissancesObjet  ( const DIA_Variable_ABC& diaVariable );
    static bool CheckTypePerceptionPoint                    ( const DIA_Variable_ABC& diaVariable );
    static bool CheckTypePerceptionLocalisation             ( const DIA_Variable_ABC& diaVariable );
    static bool CheckTypePerceptionRadar                    ( const DIA_Variable_ABC& diaVariable );
    static bool CheckTypePerceptionSurveillance             ( const DIA_Variable_ABC& diaVariable );
    static bool CheckTypePerceptionObjectsLocalisation      ( const DIA_Variable_ABC& diaVariable );   
    static bool CheckTypePerceptionFlyingShell              ( const DIA_Variable_ABC& diaVariable );   
    static bool CheckTypeMaintenancePriorites               ( const DIA_Variable_ABC& diaVariable );   
    static bool CheckTypeSantePriorites                     ( const DIA_Variable_ABC& diaVariable ); 
    static bool CheckTypeDotation                           ( const DIA_Variable_ABC& diaVariable ); 
    //@}

    //! @name Accessors
    //@{
    static const DIA_TypeDef& GetTypeLocalisation                   ();
    static const DIA_TypeDef& GetTypeConnaissanceAgent              ();
    static const DIA_TypeDef& GetTypeConnaissanceObjet              ();
    static const DIA_TypeDef& GetTypeConnaissancePopulation         ();
    static const DIA_TypeDef& GetTypePopulationConnaissanceAgent    ();
    static const DIA_TypeDef& GetTypePopulationConnaissanceObjet    ();
    static const DIA_TypeDef& GetTypeListePoints                    ();
    static const DIA_TypeDef& GetTypeItineraire                     ();
    static const DIA_TypeDef& GetTypeAction                         ();
    static const DIA_TypeDef& GetTypeLima                           ();
    static const DIA_TypeDef& GetTypePoint                          ();
    static const DIA_TypeDef& GetTypeDirection                      ();
    static const DIA_TypeDef& GetTypeID                             ();
    static const DIA_TypeDef& GetTypeCalculLignesAvantArriere       ();
    static const DIA_TypeDef& GetTypePerceptionPoint                ();
    static const DIA_TypeDef& GetTypePerceptionLocalisation         ();
    static const DIA_TypeDef& GetTypePerceptionRadar                ();
    static const DIA_TypeDef& GetTypePerceptionSurveillance         ();
    static const DIA_TypeDef& GetTypePerceptionObjectsLocalisation  ();
    static const DIA_TypeDef& GetTypePerceptionFlyingShell          ();
    static const DIA_TypeDef& GetTypeMaintenancePriorites           ();
    static const DIA_TypeDef& GetTypeSantePriorites                 ();    
    static const DIA_TypeDef& GetTypeDotation                       ();
    //@}

private:
    static const DIA_TypeDef* pTypePoint_;
    static const DIA_TypeDef* pTypeDirection_;
    static const DIA_TypeDef* pTypeItineraire_;
    static const DIA_TypeDef* pTypeListePoints_;
    static const DIA_TypeDef* pTypeLocalisation_;
    static const DIA_TypeDef* pTypeCalculLignesAvantArriere_;
    static const DIA_TypeDef* pTypeLima_;
    static const DIA_TypeDef* pTypeConnaissanceObjet_;
    static const DIA_TypeDef* pTypeConnaissancePopulation_;
    static const DIA_TypeDef* pTypeConnaissanceAgent_;
    static const DIA_TypeDef* pTypePopulationConnaissanceAgent_;
    static const DIA_TypeDef* pTypePopulationConnaissanceObjet_;
    static const DIA_TypeDef* pTypeID_;
    static const DIA_TypeDef* pTypeAction_;
    static const DIA_TypeDef* pTypePion_;
    static const DIA_TypeDef* pTypeAutomate_;
    static const DIA_TypeDef* pTypeGenObjet_;
    static const DIA_TypeDef* pTypeMissionPion_;
    static const DIA_TypeDef* pTypePerceptionPoint_;
    static const DIA_TypeDef* pTypePerceptionLocalisation_;
    static const DIA_TypeDef* pTypePerceptionRadar_;
    static const DIA_TypeDef* pTypePerceptionSurveillance_;
    static const DIA_TypeDef* pTypePerceptionObjectsLocalisation_;
    static const DIA_TypeDef* pTypeMaintenancePriorites_;
    static const DIA_TypeDef* pTypeSantePriorites_;
    static const DIA_TypeDef* pTypePerceptionFlyingShell_;
    static const DIA_TypeDef* pTypeDotation_;
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

template< class FunctionCall, class CallerType >
static void DEC_RegisterDIACallFunctor( DIA_FunctionTable< CallerType >& funcTable, FunctionCall function, const std::string& strFunctionName )
{
//    if( MIL_AgentServer::GetWorkspace().GetConfig().IsProfilingEnabled() )
//        funcTable.RegisterFunction( DEC_DIACallFunctorWithProfiling< FunctionCall, CallerType >( function ), strFunctionName );
//    else
        funcTable.RegisterFunction( DEC_DIACallFunctor< FunctionCall, CallerType >( function ), strFunctionName );
}
//@}

#include "DEC_Tools.inl"


#endif // __DEC_Tools_h_
