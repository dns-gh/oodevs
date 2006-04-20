// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Agent_h_
#define __Agent_h_

#include "ASN_Types.h"
#include "Agent_ABC.h"
#include "Resolver_ABC.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "Resolver.h"
#include "Drawable_ABC.h"
#include "Aggregatable_ABC.h"

class Controller;
class AgentType;
class AutomatType;
class KnowledgeGroup;
class DecisionalModel;
class Team;

// =============================================================================
/** @class  Agent
    @brief  Agent
*/
// Created: AGE 2006-02-13
// =============================================================================
class Agent : public Agent_ABC
            , public Extension_ABC
            , public Updatable_ABC< ASN1T_MsgChangeAutomateAck >
            , public Updatable_ABC< ASN1T_MsgChangeAutomate >
            , public Updatable_ABC< ASN1T_MsgChangeGroupeConnaissanceAck >
            , public Aggregatable_ABC
            , public Resolver< Agent > // $$$$ AGE 2006-02-16: Split Agent and Automat
            , public Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Agent( const ASN1T_MsgAutomateCreation& message,
                    Controller& controller, 
                    const Resolver_ABC< AutomatType >& resolver,
                    const Resolver_ABC< Agent >& agentResolver, 
                    const Resolver_ABC< KnowledgeGroup >& gtiaResolver );
             Agent( const ASN1T_MsgPionCreation& message,
                    Controller& controller, 
                    const Resolver_ABC< AgentType >& resolver,
                    const Resolver_ABC< Agent >& agentResolver, 
                    const Resolver_ABC< KnowledgeGroup >& gtiaResolver );
    virtual ~Agent();
    //@}

    //! @name Operations
    //@{
    // $$$$ AGE 2006-03-14: Gets de merde 
    const Team& GetTeam() const;
    virtual std::string GetName() const;
    virtual unsigned long GetId() const;
    
    KnowledgeGroup& GetKnowledgeGroup() const;

    const DecisionalModel& GetDecisionalModel() const;
    const DecisionalModel* GetAutomatDecisionalModel() const;
    const Agent* GetSuperior() const;

    const AutomatType* GetAutomatType() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Agent( const Agent& );            //!< Copy constructor
    Agent& operator=( const Agent& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgChangeAutomateAck& message );
    virtual void DoUpdate( const ASN1T_MsgChangeAutomate& message );
    virtual void DoUpdate( const ASN1T_MsgChangeGroupeConnaissanceAck& message );

    void ChangeKnowledgeGroup( unsigned long id );
    void ChangeKnowledgeGroup( KnowledgeGroup& gtia );
    void ChangeSuperior( unsigned long id );
    void AddChild   ( Agent& child );
    void RemoveChild( Agent& child );
    virtual void Aggregate( const bool& );

    void Draw( const geometry::Point2f& where, const GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    const Resolver_ABC< Agent >& agentResolver_;
    const Resolver_ABC< KnowledgeGroup >&  gtiaResolver_;
    std::string   name_;
    unsigned long id_;

    // Automat only
    AutomatType*  automatType_;
    AgentType*    type_;

    // Agent only
    Agent*  superior_;

    // Automat only
    KnowledgeGroup* gtia_;

    bool aggregated_;
    //@}
};

#endif // __Agent_h_
