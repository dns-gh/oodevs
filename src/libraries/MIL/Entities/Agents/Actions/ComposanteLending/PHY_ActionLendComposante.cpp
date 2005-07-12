// *****************************************************************************
//
// $Created: JVT 2005-05-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/ComposanteLending/PHY_ActionLendComposante.cpp $
// $Author: Jvt $
// $Modtime: 12/05/05 15:25 $
// $Revision: 1 $
// $Workfile: PHY_ActionLendComposante.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "PHY_ActionLendComposante.h"

#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionLendComposante constructor
// Created: JVT 2005-05-12
// -----------------------------------------------------------------------------
PHY_ActionLendComposante::PHY_ActionLendComposante( MIL_AgentPion& pion, DIA_Call_ABC& call )
    : PHY_Action_ABC    ( pion, call)
    , role_             ( pion.GetRole< PHY_RolePion_Composantes >() )
    , nNbrComposantes_  ( (uint)call.GetParameter( 1 ).ToFloat() )
    , pTarget_          ( 0 )
    , diaReturnVariable_( call.GetParameter( 2 ) )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );

    DEC_RolePion_Decision* pAgent = call.GetParameter( 0 ).ToUserObject( pAgent );
    assert( pAgent );
    
    pTarget_ = &pAgent->GetPion().GetRole< PHY_RolePion_Composantes >();
    
    nTimer_ = role_.GetLendCollectionComposantesTime( *pTarget_, nNbrComposantes_ );
    
    diaReturnVariable_.SetValue( false );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionLendComposante destructor
// Created: JVT 2005-05-12
// -----------------------------------------------------------------------------
PHY_ActionLendComposante::~PHY_ActionLendComposante()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionLendComposante::Execute
// Created: JVT 2005-05-12
// -----------------------------------------------------------------------------
void PHY_ActionLendComposante::Execute()
{
    if ( !nTimer_-- )
    {
        assert( pTarget_ );
        role_.LendCollectionComposantes( *pTarget_, nNbrComposantes_ );
        diaReturnVariable_.SetValue( true );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionLendComposante::ExecuteSuspended
// Created: JVT 2005-05-12
// -----------------------------------------------------------------------------
void PHY_ActionLendComposante::ExecuteSuspended()
{
    assert( false );
    Execute();
}
