// *****************************************************************************
//
// $Created: NLD 2004-07-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Decision/Functions/DEC_OrdersFunctions.h $
// $Author: Nld $
// $Modtime: 2/12/04 10:15 $
// $Revision: 3 $
// $Workfile: DEC_OrdersFunctions.h $
//
// *****************************************************************************

#ifndef __DEC_OrdersFunctions_h_
#define __DEC_OrdersFunctions_h_

#include "Entities/Orders/MIL_LimaOrder.h"
#include "Entities/Orders/MIL_LimaFunction.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Decision/DEC_Tools.h"

class DEC_Decision_ABC;
class MIL_AgentPion;
class MIL_Automate;
class MIL_Mission_ABC;
class MIL_AutomateMission;
class MIL_Fuseau;
class MT_Vector2D;
class MIL_LimaOrder;

// =============================================================================
// Created: NLD 2004-07-08
// =============================================================================
class DEC_OrdersFunctions
{
public:
    // Mission
    template< typename T > static void FinishMission      ( T& caller );
    template< typename T > static bool IsNewMissionStarted( T& caller );

    // Limas
    template< typename T > static unsigned int GetLima                   ( const T& caller, unsigned int limaId );
    template< typename T > static std::vector< unsigned int > GetLimasFromType ( const T& caller, unsigned int limaId );
    template< typename T > static MIL_LimaOrder* GetNextScheduledLima    ( const T& caller );
                           static const MIL_Fuseau& GetFuseau            ( const DEC_Decision_ABC& caller );
    template< typename T > static bool GetMissionLimaFlag                ( const T& caller, unsigned int limaId );
                           static void PionSetMissionLimaFlag            ( MIL_AgentPion& caller, unsigned int limaId, bool flag );
                           static void PionSetMissionLimaScheduleFlag    ( MIL_AgentPion& caller, unsigned int limaId, bool flag  );
                           static void AutomateSetMissionLimaFlag        ( MIL_Automate& caller, unsigned int limaId, bool flag  );
                           static void AutomateSetMissionLimaScheduleFlag( MIL_Automate& caller, unsigned int limaId, bool flag  );

    // Automate
    static boost::shared_ptr< MIL_Mission_ABC > MRT_CreatePionMission  ( MIL_Automate& callerAutomate, DEC_Decision_ABC* pPion, const std::string& mission );
    static void             MRT_Validate           ( MIL_Automate& callerAutomate );
    static void             MRT_AffectFuseaux      ( MIL_Automate& callerAutomate, std::vector< DEC_Decision_ABC* > pions );
    static boost::shared_ptr< MIL_Mission_ABC > CDT_CreatePionMission  ( MIL_Automate& callerAutomate, DEC_Decision_ABC* pPion, const std::string& mission );
    static boost::shared_ptr< MIL_Mission_ABC > CreatePionMissionBM    ( DEC_Decision_ABC* pPion, const std::string& mission );
    static boost::shared_ptr< MIL_Mission_ABC > CreatePionMissionVersPionBM    ( DEC_Decision_ABC* pPion, const std::string& mission );
    static void             CDT_GivePionMission    ( MIL_Automate& callerAutomate, boost::shared_ptr< MIL_Mission_ABC > pMission );
    static void             CDT_GivePionMissionVersPion    ( MIL_Automate& callerAutomate, boost::shared_ptr< MIL_Mission_ABC > pMission );
    static void             CDT_GiveMission    ( MIL_AgentPion& callerPion, boost::shared_ptr< MIL_Mission_ABC > pMission );
    static void             CDT_GiveMissionVersPion    ( MIL_AgentPion& callerPion, boost::shared_ptr< MIL_Mission_ABC > pMission );
    static boost::shared_ptr< MIL_Mission_ABC > CreateAutomateMission  ( MIL_Automate& callerAutomate, DEC_Decision_ABC* pAutomate, const std::string& mission  );
    static void AssignFuseauToPionMission       ( MIL_Fuseau* pFuseau, boost::shared_ptr< MIL_Mission_ABC > pMission );
    static void AssignFuseauToAutomateMission   ( MIL_Fuseau* pFuseau, boost::shared_ptr< MIL_Mission_ABC > pMission );
    static void AssignDirectionToAutomateMission( MT_Vector2D* pDirection, boost::shared_ptr< MIL_Mission_ABC > pMission);
    static void GiveAutomateMission             ( boost::shared_ptr< MIL_Mission_ABC > pMission, MIL_Automate& callerAutomate );
    static void GiveAutomateMissionToAutomat    ( boost::shared_ptr< MIL_Mission_ABC > pMission, MIL_Automate& callerAutomate );
    static void GiveMissionToAutomat            ( boost::shared_ptr< MIL_Mission_ABC > pMission );
    static std::list<MIL_Fuseau*> SplitFuseau   ( MIL_Automate& callerAutomate, unsigned int nbrSubFuseaux );

    static bool IsPionMissionAvailable( DEC_Decision_ABC* agent, std::string diaType );
    static bool IsAutomateMissionAvailable( DEC_Decision_ABC* agent, std::string diaType );
    static bool IsFragOrderAvailable( DEC_Decision_ABC* agent, const std::string& fragorder );

    static bool DEC_Mission_IsPath( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter );

private:
    //! @name Tools
    //@{
    static MIL_Automate& GetHigherEngagedAutomate( MIL_Automate& automate );
    //@}
};

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::GetMissionLimaFlag
// Created: NLD 2003-12-23
// -----------------------------------------------------------------------------
template< typename T >
bool DEC_OrdersFunctions::GetMissionLimaFlag( const T& caller, unsigned int limaID)
{
    MIL_LimaOrder* pLima = caller.GetOrderManager().FindLima( limaID );
    if( !pLima )
    {
        return false;
    }
    return pLima->IsFlagged();
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::GetLima
// Created: NLD 2004-05-21
// -----------------------------------------------------------------------------
template< typename T >
unsigned int DEC_OrdersFunctions::GetLima( const T& caller, unsigned int limaId )
{
    const MIL_LimaFunction* pFunction = MIL_LimaFunction::Find( limaId );
    if( !pFunction )
        return 0;
    MIL_LimaOrder* pLima = caller.GetOrderManager().FindLima( *pFunction );
    if( !pLima )
        return 0;
    return pLima->GetID();
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::GetLimasFromType
// Created: NMI 2013-04-30
// -----------------------------------------------------------------------------
template< typename T >
std::vector< unsigned int > DEC_OrdersFunctions::GetLimasFromType( const T& caller, unsigned int limaId )
{
    std::vector< unsigned int > vecIDs;
    const MIL_LimaFunction* pFunction = MIL_LimaFunction::Find( limaId );
    if( !pFunction )
        return vecIDs;
    std::vector< MIL_LimaOrder* > vecLimas = caller.GetOrderManager().FindAllLimas( *pFunction );
    for( auto it = vecLimas.begin(); it != vecLimas.end(); ++it )
        vecIDs.push_back( (*it)->GetID() );
    return vecIDs;
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::GetNextScheduledLima
// Created: NLD 2007-04-30
// -----------------------------------------------------------------------------
template< typename T >
MIL_LimaOrder* DEC_OrdersFunctions::GetNextScheduledLima( const T& caller )
{
    return caller.GetOrderManager().FindNextScheduledLima();
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::FinishMission
// Created: NLD 2005-09-13
// -----------------------------------------------------------------------------
template< typename T >
void DEC_OrdersFunctions::FinishMission( T& caller )
{
    caller.GetOrderManager().CancelMission();
}

// -----------------------------------------------------------------------------
// Name: DEC_OrdersFunctions::IsNewMissionStarted
// Created: NLD 2005-09-19
// -----------------------------------------------------------------------------
template< typename T >
bool DEC_OrdersFunctions::IsNewMissionStarted( T& caller )
{
    return caller.GetOrderManager().IsNewMissionStarted();
}

#endif // __DEC_OrdersFunctions_h_
