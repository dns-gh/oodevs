//*****************************************************************************
//
// $Created: AGN 03-04-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentKnowledge.h $
// $Author: Age $
// $Modtime: 31/03/05 17:36 $
// $Revision: 5 $
// $Workfile: AgentKnowledge.h $
//
//*****************************************************************************

#ifndef __AgentKnowledge_h_
#define __AgentKnowledge_h_

#include "ASN_Types.h"
#include "Entity_ABC.h"
#include "IDManager.h"
#include "Perception_Def.h"
#include "OptionalValue.h"
#include "Resolver_ABC.h"

class Agent;
class Controller;
class Displayer_ABC;
class Team;

// =============================================================================
/** @class  AgentKnowledge
    @brief  Represents the knowledge a gtia has of an agent.
*/
// Created: APE 2004-03-10
// =============================================================================
class AgentKnowledge : public Entity_ABC
                     , private Extension_ABC
                     , public Updatable_ABC< ASN1T_MsgUnitKnowledgeUpdate >
{

public:
    //! @name Constructor / Destructor
    //@{
             AgentKnowledge( const ASN1T_MsgUnitKnowledgeCreation& message, Controller& controller, 
                             const Resolver_ABC< Agent >& resolver, const Resolver_ABC< Team >& teamResolver );
    virtual ~AgentKnowledge();
    //@}

    //! @name operations
    //@{
    void Display( Displayer_ABC& displayer ) const;
    //@}

    //! @name Accessors
    //@{
    unsigned long      GetId       () const;
    Agent&             GetRealAgent() const;
    const MT_Vector2D& GetPosition () const;
    // $$$$ AGE 2006-02-21: Kg ?
    //@}

private:
   

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitKnowledgeUpdate& message );
    //@}

public:
    Controller& controller_;
    const Resolver_ABC< Agent >& resolver_;
    const Resolver_ABC< Team >& teamResolver_;
    unsigned long nID_;
    Agent&      realAgent_;
    const Team* team_;
    MT_Vector2D vPosition_;
    
    // $$$$ AGE 2006-02-22: Use Attributes ?
    OptionalValue< std::string >                strPosition_;
    OptionalValue< uint >                       nDirection_;
    OptionalValue< uint >                       nSpeed_;
    OptionalValue< uint >                       nEtatOps_;
    OptionalValue< E_PerceptionResult >         nCurrentPerceptionLevel_;
    OptionalValue< E_PerceptionResult >         nMaxPerceptionLevel_;
    OptionalValue< E_NatureLevel >              nLevel_;
    OptionalValue< E_UnitNatureWeapon >         nWeapon_;
    OptionalValue< E_UnitNatureSpecialization > nSpecialization_;
    OptionalValue< E_UnitNatureQualifier >      nQualifier_;
    OptionalValue< E_UnitNatureCategory >       nCategory_;
    OptionalValue< E_UnitNatureMobility >       nMobility_;
    OptionalValue< E_UnitCapaciteMission >      nCapacity_;
    OptionalValue< bool >                       bIsPC_;
    OptionalValue< bool >                       bSurrendered_;
    OptionalValue< bool >                       bPrisonner_;
    OptionalValue< bool >                       bRefugies_;
    OptionalValue< uint >                       nRelevance_;

private:
    static IDManager idManager_;
};

#endif // __AgentKnowledge_h_