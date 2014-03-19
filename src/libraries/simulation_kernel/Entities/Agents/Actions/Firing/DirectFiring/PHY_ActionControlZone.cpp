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
#include "Decision/DEC_Decision_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Objects/MIL_ObjectBuilder_ABC.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/Object.h"
#include "Entities/Objects/ControlZoneCapacity.h"
#include "Entities/Actions/PHY_FireResults_Default.h"
#include "Entities/MIL_EntityManager.h"
#include "Decision/DEC_Tools.h"
#include "MIL_AgentServer.h"
#include "simulation_terrain/TER_Localisation.h"
#include "MT_Tools/MT_Logger.h"

using namespace firing;

namespace
{
    class ControlZoneType : public MIL_ObjectType_ABC
    {
    public:
        virtual unsigned int GetID() const { return 0; }
        virtual const std::string& GetName() const { return name_; }
        virtual const std::string& GetRealName() const { return realName_; }
        virtual double GetPointSize() const { return 250.; }
    private:
        static const std::string realName_;
        static const std::string name_;
    };

    const std::string ControlZoneType::name_ = "control zone";
    const std::string ControlZoneType::realName_ = "control zone";

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

        virtual void Build( MIL_Object_ABC& object ) const
        {
            object.Initialize( location_ );
            object.AddCapacity( new ControlZoneCapacity( pion_ ) );
        }

    private:
        ControlZoneType type_;
        const TER_Localisation& location_;
        MIL_Agent_ABC& pion_;
    };
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionControlZone constructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionControlZone::PHY_ActionControlZone( MIL_AgentPion& pion, const TER_Localisation* pLocalisation, double rRadius, bool bFire )
    : PHY_Action_ABC()
    , rolePerceiver_   ( pion.GetRole< PHY_RoleInterface_Perceiver >() )
    , roleDirectFiring_( pion.GetRole< PHY_RoleAction_DirectFiring >() )
    , pFireResult_     ( 0 )
    , pZoneControlled_ ( 0 )
    , perceptionZoneID_( 0 )
{
    // Fire
    if( bFire )
    {
        //{ TODO
        MIL_ObjectControlZoneBuilder builder( pion, *pLocalisation );
        try
        {
            pZoneControlled_ = MIL_AgentServer::GetWorkspace().GetEntityManager().CreateObject( &pion.GetArmy(), builder );
        }
        catch( const std::exception& e )
        {
            MT_LOG_ERROR_MSG( tools::GetExceptionMsg( e ) );
        }
        //@}
    }
    // Detection
    if( pLocalisation->GetArea() <= rRadius * rRadius * MT_PI )
        perceptionZoneID_ = rolePerceiver_.EnableControlLocalisation( *pLocalisation, pion.GetRole< DEC_Decision_ABC >() );
    else
        perceptionZoneID_ = rolePerceiver_.EnableRecoLocalisation( *pLocalisation, static_cast< float >( rRadius ), pion.GetRole< DEC_Decision_ABC >() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionControlZone destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionControlZone::~PHY_ActionControlZone()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionControlZone::Stop
// Created: LDC 2010-03-11
// -----------------------------------------------------------------------------
void PHY_ActionControlZone::Stop()
{
    rolePerceiver_.DisableRecoLocalisation( perceptionZoneID_ );
    if( pFireResult_ )
        pFireResult_->DecRef();
    if( pZoneControlled_ )
        pZoneControlled_->MarkForDestruction();
    pZoneControlled_ = 0;
    pFireResult_ = 0;
    perceptionZoneID_ = 0;
}

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
