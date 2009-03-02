// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Decision.h"
#include "MIL_AgentServer.h"
#include "Entities/Orders/MIL_Report.h"
#include "MT_Tools/MT_CrashHandler.h"

// -----------------------------------------------------------------------------
// Name: DEC_Decision constructor
// Created: LDC 2009-02-27
// -----------------------------------------------------------------------------
template <class T>
DEC_Decision<T>::DEC_Decision( T& entity, const std::string& type )
: DEC_Decision_ABC( entity )
, DIA_Engine      ( *DIA_TypeManager::Instance().GetType( type ), "" )
, pEntity_        ( &entity )
{    
    defaultBehaviorParameters_.SetOwnerShip( true );
    pDefaultParameter_.reset( new DIA_Variable_Id() );
    defaultBehaviorParameters_.AddParam( pDefaultParameter_.get() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision constructor
// Created: LDC 2009-02-27
// -----------------------------------------------------------------------------
template <class T>
DEC_Decision<T>::DEC_Decision( const std::string& type )
: DIA_Engine( *DIA_TypeManager::Instance().GetType( type ), "" )
, pEntity_  ( 0 )
{    
    defaultBehaviorParameters_.SetOwnerShip( true );
    pDefaultParameter_.reset( new DIA_Variable_Id() );
    defaultBehaviorParameters_.AddParam( pDefaultParameter_.get() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision destructor
// Created: LDC 2009-02-27
// -----------------------------------------------------------------------------
template <class T>
DEC_Decision<T>::~DEC_Decision()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::UpdateDecision
// Created: LDC 2009-02-27
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::UpdateDecision()
{
    __try
    {
        PrepareUpdate    ();
        UpdateMotivations( (float)MIL_AgentServer::GetWorkspace().GetTimeStepDuration() );
        UpdateDecisions  ();
        ExecuteAllActions();
    }
    __except( MT_CrashHandler::ExecuteHandler( GetExceptionInformation() ) )
    {
        HandleUpdateDecisionError();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::Reset
// Created: LDC 2009-02-27
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::Reset()
{
    StopDefaultBehavior ();
    StartDefaultBehavior();
    assert( pMotivationTool_ );
    static_cast< DIA_Motivation_Part& >( *pMotivationTool_ ).Reset();
}

//-----------------------------------------------------------------------------
// Name: DEC_Decision::GetBehaviorPart
// Created: NLD 2002-12-12
//-----------------------------------------------------------------------------
template <class T>
DIA_BehaviorPart& DEC_Decision<T>::GetBehaviorPart() const
{
    assert( pBehaviorTool_ != 0 );
    return( static_cast< DIA_BehaviorPart& >( *pBehaviorTool_ ) );
}

//-----------------------------------------------------------------------------
// Name: DEC_Decision::GetKnowledgePart
// Created: AGN 02-12-19
//-----------------------------------------------------------------------------
template <class T>
DIA_Knowledge_Part& DEC_Decision<T>::GetKnowledgePart() const
{
    assert( pKnowledgeTool_ != 0 );
    return( static_cast< DIA_Knowledge_Part& > (*pKnowledgeTool_) );
}


// -----------------------------------------------------------------------------
// Name: DEC_Decision::HandleUpdateDecisionError
// Created: LDC 2009-03-02
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::HandleUpdateDecisionError()
{
    assert( pEntity_ );
    LogCrash();
    CleanStateAfterCrash();
    MIL_Report::PostEvent( *pEntity_, MIL_Report::eReport_MissionImpossible_ );
    pEntity_->GetOrderManager().ReplaceMission();               
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::LogCrash
// Created: LDC 2009-03-02
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::LogCrash()
{
    MT_LOG_ERROR_MSG( "Entity " << pEntity_->GetID() << "('" << pEntity_->GetName() << "') : Mission '" << pEntity_->GetOrderManager().GetMissionName() << "' impossible" );
}

// =============================================================================
// DEFAULT BEHAVIOR
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Decision::StartDefaultBehavior
// Created: LDC 2009-03-02
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::StartDefaultBehavior()
{
    if ( IsDefaultBehaviorAvailable() )
    {
        defaultBehaviorParameters_.GetParameter( 0 ).SetValue( 0 ); 
        DIA_ActivateOrder( &GetBehaviorPart(), "BEH_Defaut", 1.0, defaultBehaviorParameters_ );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::StopDefaultBehavior
// Created: LDC 2009-03-02
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::StopDefaultBehavior()
{
    if ( IsDefaultBehaviorAvailable() )
        DIA_DesactivateOrder( &GetBehaviorPart(), "BEH_Defaut", defaultBehaviorParameters_, true );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::IsDefaultBehaviorAvailable
// Created: LDC 2009-03-02
// -----------------------------------------------------------------------------
template <class T>
bool DEC_Decision<T>::IsDefaultBehaviorAvailable() const
{
    return GetBehaviorPart().FindBehavior( "BEH_Defaut" ) != 0;
}