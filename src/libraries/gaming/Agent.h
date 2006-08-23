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
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/DataDictionary.h"

namespace kernel
{
    class Controller;
    class AgentType;
    class AutomatType;
}

// =============================================================================
/** @class  Agent
    @brief  Agent
*/
// Created: AGE 2006-02-13
// =============================================================================
class Agent : public kernel::Agent_ABC
            , public kernel::Extension_ABC
            , public kernel::Aggregatable_ABC
            , public kernel::Drawable_ABC
            , public kernel::Updatable_ABC< ASN1T_MsgChangeAutomateAck >
            , public kernel::Updatable_ABC< ASN1T_MsgChangeAutomate >
            , public kernel::Updatable_ABC< ASN1T_MsgChangeGroupeConnaissanceAck >
{

public:
    //! @name Constructors/Destructor
    //@{
             Agent( const ASN1T_MsgAutomateCreation& message,
                    kernel::Controller& controller, 
                    const kernel::Resolver_ABC< kernel::AutomatType >& resolver,
                    const kernel::Resolver_ABC< kernel::Agent_ABC >& agentResolver, 
                    const kernel::Resolver_ABC< kernel::KnowledgeGroup_ABC >& gtiaResolver );
             Agent( const ASN1T_MsgPionCreation& message,
                    kernel::Controller& controller, 
                    const kernel::Resolver_ABC< kernel::AgentType >& resolver,
                    const kernel::Resolver_ABC< kernel::Agent_ABC >& agentResolver, 
                    const kernel::Resolver_ABC< kernel::KnowledgeGroup_ABC >& gtiaResolver );
    virtual ~Agent();
    //@}

    //! @name Operations
    //@{
    virtual bool IsInTeam( const kernel::Team_ABC& team ) const;
    virtual const kernel::Team_ABC& GetTeam() const;
    virtual kernel::KnowledgeGroup_ABC& GetKnowledgeGroup() const;
    virtual const kernel::Agent_ABC* GetSuperior() const;

    virtual QString GetName() const;
    virtual unsigned long GetId() const;

    virtual const kernel::AutomatType* GetAutomatType() const;
    virtual const kernel::AgentType& GetType() const;
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
    void ChangeKnowledgeGroup( kernel::KnowledgeGroup_ABC& gtia );
    void ChangeSuperior( unsigned long id );
    virtual void Aggregate( const bool& );

    void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;

    void RemoveChild( kernel::Agent_ABC& child );
    void AddChild( kernel::Agent_ABC& child );

    void CreateDictionary();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Resolver_ABC< kernel::Agent_ABC >& agentResolver_;
    const kernel::Resolver_ABC< kernel::KnowledgeGroup_ABC >&  gtiaResolver_;
    unsigned long id_;
    QString   name_;

    // Automat only
    kernel::AutomatType*  automatType_;
    kernel::AgentType*    type_;

    // Agent only
    Agent*  superior_;

    // Automat only
    kernel::KnowledgeGroup_ABC* gtia_;

    bool aggregated_;
    //@}
};

#endif // __Agent_h_
