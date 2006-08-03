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
#include "DataDictionary.h"

class Controller;
class AgentType;
class AutomatType;
class DecisionalModel;

// =============================================================================
/** @class  Agent
    @brief  Agent
*/
// Created: AGE 2006-02-13
// =============================================================================
class Agent : public Agent_ABC
            , public Updatable_ABC< ASN1T_MsgChangeAutomateAck >
            , public Updatable_ABC< ASN1T_MsgChangeAutomate >
            , public Updatable_ABC< ASN1T_MsgChangeGroupeConnaissanceAck >
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
    virtual bool IsInTeam( const Team& team ) const;
    virtual const Team& GetTeam() const;
    virtual KnowledgeGroup& GetKnowledgeGroup() const;

    virtual std::string GetName() const;
    virtual unsigned long GetId() const;

    const DecisionalModel& GetDecisionalModel() const;
    const DecisionalModel* GetAutomatDecisionalModel() const;
    const Agent* GetSuperior() const;

    const AutomatType* GetAutomatType() const;

    virtual void Select( ActionController& controller ) const;
    virtual void ContextMenu( ActionController& controller, const QPoint& where ) const;
    virtual void Activate( ActionController& controller ) const;
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

    void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const;

    void CreateDictionary();
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
