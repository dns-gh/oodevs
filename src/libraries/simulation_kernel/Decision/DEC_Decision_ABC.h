// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __DEC_Decision_ABC_h_
#define __DEC_Decision_ABC_h_

#include "MT_Tools/Role_ABC.h"
#include <directia/tools/binders/ScriptRef.h>

namespace sword
{
    class Brain;
}

class DEC_AutomateDecision;
class DEC_Knowledge_Agent;
class DEC_Knowledge_Object;
class MIL_AgentPion;
class MIL_Automate;
class MIL_FragOrderType;
class MIL_KnowledgeGroup;
class MIL_Mission_ABC;
class MIL_MissionType_ABC;
class MIL_OrderManager_ABC;

class MT_Vector2D;
class PHY_Action_ABC;
class PHY_DotationCategory;

// =============================================================================
/** @class  DEC_Decision_ABC
    @brief  DEC_Decision_ABC
*/
// Created: RDS 2008-04-30
// =============================================================================
class DEC_Decision_ABC : public tools::Role_ABC
{
public:
    //! @name Types
    //@{
    typedef DEC_Decision_ABC RoleInterface;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Decision_ABC();
    virtual ~DEC_Decision_ABC();
    //@}

public:
    //! @name Operations
    //@{
    virtual void UpdateDecision( float duration ) = 0;
    virtual void SetMission( boost::shared_ptr< MIL_Mission_ABC > pMission ) = 0;
    virtual boost::shared_ptr< MIL_Mission_ABC > GetMission() = 0;
    virtual DEC_AutomateDecision* GetDecAutomate() = 0;
    virtual std::string GetName() const = 0;
    virtual void RemoveCallback( unsigned int actionId ) = 0;
    template< typename T > void Callback( unsigned int actionId, T value );
    virtual void CallbackKnowledge( unsigned int, boost::shared_ptr< DEC_Knowledge_Object > value ) = 0;
    virtual void CallbackPerception( int id ) = 0;
    virtual const std::string& GetDIAType() const = 0;
    virtual MIL_AgentPion& GetPion() const = 0;
    virtual MIL_Automate& GetAutomate() const = 0;
    virtual unsigned int GetID() const = 0;
    virtual boost::shared_ptr< MIL_KnowledgeGroup > GetKnowledgeGroup() const = 0;
    virtual MIL_OrderManager_ABC& GetOrderManager() const = 0;

    virtual void GarbageCollect() = 0;
    virtual void Reload( bool doInitBrain ) = 0;
    virtual void IncDIARef() = 0;
    virtual void DecDIARef() = 0;
    virtual bool IsUsedByDIA() const = 0;
    virtual void ActivateBrainDebug( bool activate ) = 0;

    virtual void StartMissionBehavior( const boost::shared_ptr< MIL_Mission_ABC > mission ) = 0;
    virtual void StopMissionBehavior ( const boost::shared_ptr< MIL_Mission_ABC > mission ) = 0;
    virtual bool IsFragOrderAvailableForMission( const MIL_MissionType_ABC& missionType, const MIL_FragOrderType& fragOrderType ) const = 0;
    virtual bool IsFragOrderAvailable( const MIL_FragOrderType& fragOrderType ) const = 0;

    virtual int  GeteEtatPhaseMission() const = 0;
    virtual void SeteEtatPhaseMission( int value ) = 0;
    virtual int  GeteEtatLima() const = 0;
    virtual void SeteEtatLima( int value ) = 0;
    virtual int  GeteEtatDec() const = 0;
    virtual void SeteEtatDec( int value ) = 0;
    virtual int  GeteEtatEchelon() const = 0;
    virtual void SeteEtatEchelon( int value ) = 0;

    virtual bool GetbOrdreDecrocher() const = 0;
    virtual void SetbOrdreDecrocher( bool value ) = 0;
    virtual bool GetbOrdreTenirSurLR() const = 0;
    virtual void SetbOrdreTenirSurLR( bool value ) = 0;
    virtual bool GetbOrdreTenir() const = 0;
    virtual void SetbOrdreTenir( bool value ) = 0;

    virtual void SetAmbianceMission( int ) = 0;
    virtual void SetAppuieFreinage( bool ) = 0;
    virtual bool GetDemandeOrdreConduitePoursuivre() = 0;
    virtual bool GetEnCoursExtractionPersonnel() = 0;
    virtual bool GetEnExploitation() = 0;
    virtual void SetbEnExploitation( bool ) = 0;
    virtual bool GetEnPhaseRavitaillement() = 0;
    virtual void SetEnPhaseRavitaillement( bool ) = 0;
    virtual bool GetMiseEnOeuvre() = 0;
    virtual void SetMiseEnOeuvre( bool ) = 0;
    virtual bool GetSuppliesLoaded() = 0;
    virtual void SetSuppliesLoaded( bool ) = 0;
    virtual int  GetEtatFeu() = 0;
    virtual std::vector< boost::shared_ptr< DEC_Knowledge_Agent > > GetListeEnisTirAutorise() = 0;
    virtual void SetListeEnisTirAutorise( const std::vector< boost::shared_ptr< DEC_Knowledge_Agent > >& ) = 0;
    virtual std::vector<DEC_Decision_ABC*> GetListePionsCoordination() = 0;
    virtual boost::shared_ptr< DEC_Knowledge_Object > GetObjMisEnCours() = 0;
    virtual void SetObjMisEnCours( boost::shared_ptr< DEC_Knowledge_Object > ) = 0;
    virtual boost::shared_ptr< MT_Vector2D > GetObjectifCourant() = 0;
    virtual MT_Vector2D* GetPtSauvegarde() = 0;
    virtual boost::shared_ptr< DEC_Knowledge_Object > GetPlotRavitaillementAssigne() = 0;
    virtual void SetPlotRavitaillementAssigne( boost::shared_ptr< DEC_Knowledge_Object > ) = 0;
    virtual int GetPorteeAction() = 0;
    virtual void SetPorteeAction( int ) = 0;
    virtual float GetNiveauAlerteRavitaillement() = 0;
    virtual void ClearListeEnisTirAutorise() = 0;
    virtual void ClearListePionsCoordination() = 0;
    virtual void ClearPlotsRavitaillement() = 0;
    virtual std::vector<DEC_Knowledge_Object*> GetListeZonesTir() = 0;
    virtual void ClearListeZonesTir() = 0;

    virtual void SetMunition( const PHY_DotationCategory* ) = 0;
    virtual const PHY_DotationCategory* GetMunition() = 0;
    virtual void SetNbIt( int value ) = 0;
    virtual int GetNbIt() = 0;
    virtual void SetTarget( boost::shared_ptr< MT_Vector2D > ) = 0;
    virtual boost::shared_ptr< MT_Vector2D >  GetTarget() = 0;

    void SetVariableInt( const std::string& name, int value );
    template< typename T > void SetVariable( const std::string& name, T value );
    template< typename T > T GetVariable( const std::string& name );

    virtual std::vector< DEC_Decision_ABC* > GetPionsWithPC() = 0;
    virtual std::vector< DEC_Decision_ABC* > GetCommunicationPionsWithPC() = 0;
    virtual bool IsNeutralized() const = 0;
    virtual bool IsMoving() const = 0;
    virtual bool IsContaminated() const = 0;
    virtual bool IsDecontaminationFinished() const = 0;
    virtual const MT_Vector2D* GetPosition() const = 0;
    virtual void SetStateVariable( const std::string& name, float value ) = 0;
    virtual bool IsPC() const = 0;
    virtual bool IsTransported() const = 0;
    virtual bool IsInCity() const = 0;
    virtual bool IsFlying() const = 0;
    virtual double GetOperationalState() const = 0;
    virtual double GetMajorOperationalState() const = 0;
    virtual bool IsAutomateEngaged() const = 0;
    virtual bool IsDead() const = 0;
    virtual bool IsJammed() const = 0;
    virtual bool IsInReceptionBlackout() const = 0;
    virtual bool IsInEmissionBlackout() const = 0;
    virtual boost::shared_ptr< MT_Vector2D > GetLastPointOfPath( const double time, bool bBoundOnPath ) const = 0;
    virtual boost::shared_ptr< MT_Vector2D > ExtrapolatePosition( const double time, bool bBoundOnPath ) const = 0;
    std::string ExecuteScript( const std::string& funcName, const std::string& script );
    //@}

    //! @name Error management
    //@{
    virtual void LogError( const std::exception* error = 0 ) const = 0;
    //@}

    //! @name Serialization
    //@{
    template< typename Archive > void serialize( Archive& ar, const unsigned int )
    {
        ar & boost::serialization::base_object< tools::Role_ABC >( *this );
    }
    //@}

private:
    //! @name Helpers
    //@{
    directia::tools::binders::ScriptRef GetScriptRef( const std::string& id );

    virtual sword::Brain* GetBrain() = 0;
    template< typename T > static void SetScriptVariable( const T& source, T& dest );
    //@}
};

// -----------------------------------------------------------------------------
// Name: DEC_Decision_ABC::Callback
// Created: LDC 2009-06-29
// -----------------------------------------------------------------------------
template< typename T >
void DEC_Decision_ABC::Callback( unsigned int actionId, T value )
{
    GetScriptRef( "CallbackAction" )( actionId, value );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision_ABC::SetVariable
// Created: LDC 2009-07-31
// -----------------------------------------------------------------------------
template <typename T>
void DEC_Decision_ABC::SetVariable( const std::string& name, T value )
{
    GetScriptRef( name ) = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision_ABC::SetScriptVariable
// Created: LDC 2009-07-31
// -----------------------------------------------------------------------------
template< typename T >
void DEC_Decision_ABC::SetScriptVariable( const T& source, T& dest )
{
    dest = source;
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision_ABC::GetVariable
// Created: LDC 2009-07-31
// -----------------------------------------------------------------------------
template< typename T >
T DEC_Decision_ABC::GetVariable( const std::string& name )
{
    directia::tools::binders::ScriptRef scriptRef = GetScriptRef( name );
    if( !scriptRef )
        scriptRef = T();
    return scriptRef;
}

#endif // __DEC_Decision_ABC_h_
