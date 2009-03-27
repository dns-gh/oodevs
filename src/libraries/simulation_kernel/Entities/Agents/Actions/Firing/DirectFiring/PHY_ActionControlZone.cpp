// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionControlZone.cpp $
// $Author: Age $
// $Modtime: 31/01/05 13:13 $
// $Revision: 5 $
// $Workfile: PHY_ActionControlZone.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_ActionControlZone.h"

#include "PHY_RoleAction_DirectFiring.h"
#include "Entities/MIL_Army.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Perception/PHY_RolePion_Perceiver.h"
#include "Entities/Objects/MIL_ObjectBuilder_ABC.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/Object.h"
#include "Entities/Objects/ControlZoneCapacity.h"
#include "Entities/Actions/PHY_FireResults_Default.h"
#include "Entities/MIL_EntityManager.h"
#include "Tools/MIL_Tools.h"
#include "Decision/DEC_Tools.h"
#include "MIL_AgentServer.h"
#include "simulation_terrain/TER_Localisation.h"

namespace
{
    class ControlZoneType : public MIL_ObjectType_ABC
    {
    public:
        virtual uint GetID() const { return 0; }
        virtual const std::string& GetName() const { return name_; }
    private:
        static const std::string name_;
    };

    const std::string ControlZoneType::name_ = "control zone";

    class MIL_ObjectControlZoneBuilder : public MIL_ObjectBuilder_ABC
    {
    public:
        MIL_ObjectControlZoneBuilder( MIL_Agent_ABC& pion, const TER_Localisation& location ) 
            : location_ ( location )
            , pion_ ( pion )
        {
        }

        const MIL_ObjectType_ABC& GetType() const 
        {
            return type_;
        }

        virtual void Build( Object& object ) const
        {
            object.Initialize( location_ );
            object.AddCapacity( new ControlZoneCapacity( pion_ ) );
        }

    private:
        ControlZoneType         type_;
        const TER_Localisation& location_;
        MIL_Agent_ABC&          pion_;
    };
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionControlZone constructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionControlZone::PHY_ActionControlZone( MIL_AgentPion& pion, DIA_Call_ABC& diaCall )
    : PHY_Action_ABC    ( pion, diaCall )
    , rolePerceiver_    ( pion.GetRole< PHY_RolePion_Perceiver      >() )
    , roleDirectFiring_ ( pion.GetRole< PHY_RoleAction_DirectFiring >() )
    , pFireResult_      ( 0 )  
    , pZoneControlled_  ( 0 )
    , pPerceptionZoneID_( 0 )
{    
    DEC_Tools::CheckTypeLocalisation( diaCall.GetParameter( 0 ) );
    
    const TER_Localisation* pLocalisation  = diaCall.GetParameter( 0 ).ToUserPtr( pLocalisation );
    const MT_Float          rRadius        = MIL_Tools::ConvertMeterToSim( diaCall.GetParameter( 1 ).ToFloat() );

    // Fire
    if( diaCall.GetParameter( 2 ).ToBool() )
    {    
        //{ TODO
        MIL_ObjectControlZoneBuilder builder( pion, *pLocalisation );
        pZoneControlled_ = MIL_AgentServer::GetWorkspace().GetEntityManager().CreateObject( pion.GetArmy(), builder );
        //@}
    }
    
    // Detection
    if ( pLocalisation->GetArea() <= rRadius * rRadius * MT_PI ) 
        pPerceptionZoneID_ = rolePerceiver_.EnableControlLocalisation( *pLocalisation );
    else
        pPerceptionZoneID_ = rolePerceiver_.EnableRecoLocalisation   ( *pLocalisation, rRadius );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionControlZone destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionControlZone::~PHY_ActionControlZone()
{
    rolePerceiver_.DisableRecoLocalisation( pPerceptionZoneID_ );

    if( pFireResult_ )
        pFireResult_->DecRef();

    if( pZoneControlled_ )
        pZoneControlled_->MarkForDestruction();

    pZoneControlled_   = 0;
    pFireResult_       = 0;
    pPerceptionZoneID_ = 0;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ActionControlZone::Execute
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionControlZone::Execute()
{   
    if( !pZoneControlled_ )
        return;

    const bool bMustRefResult = ( pFireResult_ == 0 );
    roleDirectFiring_.FireZone( *pZoneControlled_, pFireResult_ );
    if( pFireResult_ && bMustRefResult )
        pFireResult_->IncRef();
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionControlZone::ExecuteSuspended
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_ActionControlZone::ExecuteSuspended()
{
    Execute();
}
