// *****************************************************************************
//
// $Created: NLD 2004-06-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/DEC_Debug.cpp $
// $Author: Nld $
// $Modtime: 19/10/04 13:54 $
// $Revision: 1 $
// $Workfile: DEC_Debug.cpp $
//
// *****************************************************************************


#include "simulation_kernel_pch.h"
#include "DEC_Debug.h"

#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Populations/MIL_Population.h"

#include "DIA/DIA_Instance.h"
#include "DIA/DIA_BasicBehavior_ABC.h"

DEC_Debug::T_RecoPerInstanceMap   DEC_Debug::recoPerInstanceMap_;
DEC_Debug::T_ActionPerInstanceMap DEC_Debug::actionPerInstanceMap_;


// -----------------------------------------------------------------------------
// Name: DEC_Debug constructor
// Created: NLD 2004-06-04
// -----------------------------------------------------------------------------
DEC_Debug::DEC_Debug()
{
    DIA_SetAssert( DIAAssert );    
}

// -----------------------------------------------------------------------------
// Name: DEC_Debug destructor
// Created: NLD 2004-06-04
// -----------------------------------------------------------------------------
DEC_Debug::~DEC_Debug()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Debug::HandlesEnd
// Created: NLD 2004-06-04
// -----------------------------------------------------------------------------
void DEC_Debug::HandlesEnd( const DIA_Instance* pInstance )
{
#ifdef _DEBUG
    DIA_Instance* pInstanceTmp = const_cast< DIA_Instance* >( pInstance );
    int nNbElements = recoPerInstanceMap_.count( (void*)pInstance );
    if( nNbElements != 0 )
    {
        MT_LOG_DEBUG_MSG( MT_FormatString( "DEC_Debug::HandlesEnd - Reco started by the instance '%s', but not stopped", pInstanceTmp->GetParent().GetName().c_str() ) );
        assert( false );
    }

    nNbElements = actionPerInstanceMap_.count( (void*)pInstance );
    if( nNbElements != 0 )
    {
        MT_LOG_DEBUG_MSG( MT_FormatString( "DEC_Debug::HandlesEnd - Action started by the instance '%s', but not stopped", pInstanceTmp->GetParent().GetName().c_str() ) );
        assert( false );
    }
#endif
}

// -----------------------------------------------------------------------------
// Name: DEC_Debug::NotifyRecoAdded
// Created: NLD 2004-06-04
// -----------------------------------------------------------------------------
void DEC_Debug::NotifyRecoAdded( DIA_Call_ABC& call, const MT_Vector2D& vCenter )
{
#ifdef _DEBUG
    void* pInstance = call.GetCallingLocation().GetCallingAdress();
    T_RangeRecoPerInstanceMap foundRange = recoPerInstanceMap_.equal_range( pInstance );
    for( IT_RecoPerInstanceMap itReco = foundRange.first; itReco != foundRange.second; ++itReco )
        if( itReco->second == vCenter )
        {
            MT_LOG_DEBUG_MSG( MT_FormatString( "DEC_Debug::NotifyRecoAdded - ALREADY REGISTERED - Center %.2f/%.2f", vCenter.rX_, vCenter.rY_ ) );
            return;
        }
    recoPerInstanceMap_.insert( std::make_pair( pInstance, vCenter ) );
#endif
}

// -----------------------------------------------------------------------------
// Name: DEC_Debug::NotifyRecoRemoved
// Created: NLD 2004-06-04
// -----------------------------------------------------------------------------
void DEC_Debug::NotifyRecoRemoved( DIA_Call_ABC& call, const MT_Vector2D& vCenter )
{
#ifdef _DEBUG
    T_RangeRecoPerInstanceMap foundRange = recoPerInstanceMap_.equal_range( call.GetCallingLocation().GetCallingAdress() );
    for( IT_RecoPerInstanceMap itReco = foundRange.first; itReco != foundRange.second; ++itReco )
        if( itReco->second == vCenter )
        {
            recoPerInstanceMap_.erase( itReco );
            return;
        }
    MT_LOG_DEBUG_MSG( MT_FormatString( "DEC_Debug::NotifyRecoRemoved - ALREADY REMOVED - Center %.2f/%.2f", vCenter.rX_, vCenter.rY_ ) );
#endif
}

// -----------------------------------------------------------------------------
// Name: DEC_Debug::NotifyActionStarted
// Created: NLD 2004-06-04
// -----------------------------------------------------------------------------
void DEC_Debug::NotifyActionStarted( DIA_Call_ABC& call, const MIL_AgentPion& pion, const PHY_Action_ABC& action )
{
#ifdef _DEBUG
    DIA_Instance* pInstance = static_cast< DIA_Instance* >( call.GetCallingLocation().GetCallingAdress() );
    const std::string& strInstance  = pInstance->GetParent().GetName();

    T_RangeActionPerInstanceMap foundRange = actionPerInstanceMap_.equal_range( pInstance );
    for( IT_ActionPerInstanceMap itAction = foundRange.first; itAction != foundRange.second; ++itAction )
        if( itAction->second == &action )
        {
            MT_LOG_DEBUG_MSG( MT_FormatString( "DEC_Debug::NotifyActionStarted - Agent %d - Instance %s - ACTION ALREADY REGISTERED - Address %p", pion.GetID(), strInstance.c_str(), &action ) );
            return;
        }
    actionPerInstanceMap_.insert( std::make_pair( pInstance, &action ) );
#endif
}

// -----------------------------------------------------------------------------
// Name: DEC_Debug::NotifyActionStarted
// Created: NLD 2004-06-04
// -----------------------------------------------------------------------------
void DEC_Debug::NotifyActionStarted( DIA_Call_ABC& call, const MIL_Population& population, const PHY_Action_ABC& action )
{
#ifdef _DEBUG
    DIA_Instance* pInstance = static_cast< DIA_Instance* >( call.GetCallingLocation().GetCallingAdress() );
    const std::string& strInstance  = pInstance->GetParent().GetName();

    T_RangeActionPerInstanceMap foundRange = actionPerInstanceMap_.equal_range( pInstance );
    for( IT_ActionPerInstanceMap itAction = foundRange.first; itAction != foundRange.second; ++itAction )
        if( itAction->second == &action )
        {
            MT_LOG_DEBUG_MSG( MT_FormatString( "DEC_Debug::NotifyActionStarted - Population %d - Instance %s - ACTION ALREADY REGISTERED - Address %p", population.GetID(), strInstance.c_str(), &action ) );
            return;
        }
    actionPerInstanceMap_.insert( std::make_pair( pInstance, &action ) );
#endif
}

// -----------------------------------------------------------------------------
// Name: DEC_Debug::NotifyActionStopped
// Created: NLD 2004-06-04
// -----------------------------------------------------------------------------
void DEC_Debug::NotifyActionStopped( DIA_Call_ABC& call, const MIL_AgentPion& pion, const PHY_Action_ABC& action )
{
#ifdef _DEBUG         
    DIA_Instance* pInstance = static_cast< DIA_Instance* >( call.GetCallingLocation().GetCallingAdress() );
    const std::string& strInstance = pInstance ->GetParent().GetName();

    T_RangeActionPerInstanceMap foundRange = actionPerInstanceMap_.equal_range( pInstance );
    for( IT_ActionPerInstanceMap itAction = foundRange.first; itAction != foundRange.second; ++itAction )
        if( itAction->second == &action )
        {
            actionPerInstanceMap_.erase( itAction );
            return;
        }
    MT_LOG_DEBUG_MSG( MT_FormatString( "DEC_Debug::NotifyActionStopped - Agent %d - Instance %s - Address %p - ACTION ALREADY UNREGISTERED", pion.GetID(), strInstance.c_str(), &action ) );
#endif
}

// -----------------------------------------------------------------------------
// Name: DEC_Debug::NotifyActionStopped
// Created: NLD 2004-06-04
// -----------------------------------------------------------------------------
void DEC_Debug::NotifyActionStopped( DIA_Call_ABC& call, const MIL_Population& population, const PHY_Action_ABC& action )
{
#ifdef _DEBUG         
    DIA_Instance* pInstance = static_cast< DIA_Instance* >( call.GetCallingLocation().GetCallingAdress() );
    const std::string& strInstance = pInstance ->GetParent().GetName();

    T_RangeActionPerInstanceMap foundRange = actionPerInstanceMap_.equal_range( pInstance );
    for( IT_ActionPerInstanceMap itAction = foundRange.first; itAction != foundRange.second; ++itAction )
        if( itAction->second == &action )
        {
            actionPerInstanceMap_.erase( itAction );
            return;
        }
    MT_LOG_DEBUG_MSG( MT_FormatString( "DEC_Debug::NotifyActionStopped - Population %d - Instance %s - Address %p - ACTION ALREADY UNREGISTERED", population.GetID(), strInstance.c_str(), &action ) );
#endif
}


//-----------------------------------------------------------------------------
// Name: DEC_Debug::DIAAssert
// Created: AGN 03-06-04
//-----------------------------------------------------------------------------
// static
void DEC_Debug::DIAAssert( const std::string& strTestExpression, const std::string& strFileName, int nLine, const std::string& strMore )
{
    //!< Assert function used by the DirectIA scripts when an expression is asserted
#ifdef _DEBUG
    std::stringstream strAssertMsg;
    strAssertMsg << "Assert in DirectIA : '" << strTestExpression << "' in file " << strFileName << " at line " << nLine << std::endl << "More: " << strMore << std::endl;
    MT_LOG_MESSAGE_MSG( strAssertMsg.str().c_str() )

    assert( false );
#endif // _DEBUG
}

