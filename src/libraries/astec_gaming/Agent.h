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
#include "astec_kernel/Agent_ABC.h"
#include "astec_kernel/DataDictionary.h"

class Controller;
class AgentType;
class AutomatType;

// =============================================================================
/** @class  Agent
    @brief  Agent
*/
// Created: AGE 2006-02-13
// =============================================================================
class Agent : public Agent_ABC
            , public Extension_ABC
            , public Aggregatable_ABC
            , public Drawable_ABC
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
                    const Resolver_ABC< Agent_ABC >& agentResolver, 
                    const Resolver_ABC< KnowledgeGroup >& gtiaResolver );
             Agent( const ASN1T_MsgPionCreation& message,
                    Controller& controller, 
                    const Resolver_ABC< AgentType >& resolver,
                    const Resolver_ABC< Agent_ABC >& agentResolver, 
                    const Resolver_ABC< KnowledgeGroup >& gtiaResolver );
    virtual ~Agent();
    //@}

    //! @name Operations
    //@{
    virtual bool IsInTeam( const Team& team ) const;
    virtual const Team& GetTeam() const;
    virtual KnowledgeGroup& GetKnowledgeGroup() const;
    virtual const Agent_ABC* GetSuperior() const;

    virtual std::string GetName() const;
    virtual unsigned long GetId() const;

    virtual const AutomatType* GetAutomatType() const;
    virtual const AgentType& GetType() const;
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
    virtual void Aggregate( const bool& );

    void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const;

    void RemoveChild( Agent_ABC& child );
    void AddChild( Agent_ABC& child );

    void CreateDictionary();
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    const Resolver_ABC< Agent_ABC >& agentResolver_;
    const Resolver_ABC< KnowledgeGroup >&  gtiaResolver_;
    unsigned long id_;
    std::string   name_;

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
