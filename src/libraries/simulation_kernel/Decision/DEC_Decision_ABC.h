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
#include <directia/Brain.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>

class DEC_AutomateDecision;
class DEC_Knowledge_Agent;
class MIL_AgentPion;
class MIL_Automate;
class MIL_Mission_ABC;
class PHY_Action_ABC;

namespace directia
{
    class Brain;
}

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
    virtual void SetMission( MIL_Mission_ABC* pMission ) = 0;
    virtual MIL_Mission_ABC* GetMission() = 0;
    virtual DEC_AutomateDecision* GetDecAutomate() const = 0;
    virtual std::string GetName() const = 0;
    virtual void RemoveCallback( PHY_Action_ABC* pAction ) = 0;
    template< typename T > void Callback( PHY_Action_ABC* pAction, T value );
    virtual void CallbackKnowledge( PHY_Action_ABC* pAction, int value ) = 0;
    virtual void CallbackPerception( int id ) = 0;
    virtual const std::string& GetDIAType() const = 0;
    virtual MIL_AgentPion& GetPion() const = 0;
    virtual MIL_Automate& GetAutomate() const = 0;
    
    virtual void StartMissionBehavior( MIL_Mission_ABC& mission ) = 0;
    virtual void StopMissionBehavior ( MIL_Mission_ABC& mission ) = 0;
    
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
    virtual int GetEtatFeu() = 0;
    virtual std::vector<DEC_Knowledge_Agent*> GetListeEnisTirAutorise() = 0;
    virtual std::vector<DEC_Decision_ABC*> GetListePionsCoordination() = 0;
    virtual int GetObjMisEnCours() = 0;
    virtual void SetObjMisEnCours( int ) = 0;
    virtual boost::shared_ptr< MT_Vector2D > GetObjectifCourant() = 0;
    virtual int GetPlotRavitaillementAssigne() = 0;
    virtual void SetPlotRavitaillementAssigne( int ) = 0;
    virtual int GetPorteeAction() = 0;
    virtual void SetPorteeAction( int ) = 0;
    virtual float GetNiveauAlerteRavitaillement() = 0;

    void SetVariableInt( const std::string& name, int value );
    template< typename T > void SetVariable( const std::string& name, T value );
    template< typename T > T GetVariable( const std::string& name );

    virtual std::vector< DEC_Decision_ABC* > GetPionsWithPC() = 0;
    virtual bool IsNeutralized() const = 0;
    virtual bool IsMoving() const = 0;
    virtual bool IsContaminated() const = 0;
    virtual const MT_Vector2D* GetPosition() const = 0;
    virtual void SetStateVariable( const std::string& name, float value ) = 0;
    virtual bool IsPC() const = 0;
    virtual bool IsTransported() const = 0;
    virtual bool IsFlying() const = 0;
    virtual MT_Float GetMajorOperationalState() const = 0;
    virtual bool IsAutomateEngaged() const = 0;
    virtual bool IsDead() const = 0;
    virtual void WearNbcProtectionSuit() const = 0;
    virtual void RemoveNbcProtectionSuit() const = 0;
    //@}

private:
    //! @name Helpers
    //@{
    virtual directia::Brain& GetBrain() = 0;
    template< typename T > static void SetScriptVariable( const T& source, T& dest );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DEC_Decision_ABC( const DEC_Decision_ABC& );            //!< Copy constructor
    DEC_Decision_ABC& operator=( const DEC_Decision_ABC& ); //!< Assignment operator
    //@}
};

#include "DEC_Decision_ABC.inl"

#endif // __DEC_Decision_ABC_h_
