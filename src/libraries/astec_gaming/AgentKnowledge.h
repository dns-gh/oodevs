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
#include "astec_kernel/Entity_ABC.h"
#include "astec_kernel/IDManager.h"
#include "astec_kernel/OptionalValue.h"
#include "astec_kernel/Resolver_ABC.h"
#include "astec_kernel/Drawable_ABC.h"

class Agent_ABC;
class Controller;
class Displayer_ABC;
class Team;
class CoordinateConverter_ABC;
class KnowledgeGroup;

// =============================================================================
/** @class  AgentKnowledge
    @brief  Represents the knowledge a gtia has of an agent.
*/
// Created: APE 2004-03-10
// =============================================================================
class AgentKnowledge : public Entity_ABC
                     , public Extension_ABC
                     , public Updatable_ABC< ASN1T_MsgUnitKnowledgeUpdate >
                     , public Drawable_ABC
{

public:
    //! @name Constructor / Destructor
    //@{
             AgentKnowledge( const KnowledgeGroup& group, const ASN1T_MsgUnitKnowledgeCreation& message, Controller& controller, 
                             const CoordinateConverter_ABC& converter, const Resolver_ABC< Agent_ABC >& resolver, const Resolver_ABC< Team >& teamResolver );
    virtual ~AgentKnowledge();
    //@}

    //! @name operations
    //@{
    void Display( Displayer_ABC& displayer ) const;
    bool IsInTeam( const Team& team ) const;
    bool KnowledgeIsInTeam( const Team& team ) const;
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const;

    virtual void Select( ActionController& controller ) const;
    virtual void ContextMenu( ActionController& controller, const QPoint& where ) const;
    virtual void Activate( ActionController& controller ) const;
    //@}

    //! @name Accessors
    //@{
    virtual unsigned long GetId       () const;
    virtual std::string   GetName     () const;
    Agent_ABC&                GetRealAgent() const;
    const Team*           GetKnowledgeTeam() const;
    const KnowledgeGroup& GetKnowledgeGroup() const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitKnowledgeUpdate& message );
    //@}

public:
    Controller& controller_;
    const CoordinateConverter_ABC& converter_;
    const Resolver_ABC< Agent_ABC >& resolver_;
    const Resolver_ABC< Team >& teamResolver_;
    const KnowledgeGroup& group_;

    unsigned long nID_;
    Agent_ABC&      realAgent_;
    const Team* team_;
    
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