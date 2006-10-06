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
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class Automat_ABC;
    class Controller;
    class AgentType;
    class AutomatType;
    class InstanciationComplete;
}

// =============================================================================
/** @class  Agent
    @brief  Agent
*/
// Created: AGE 2006-02-13
// =============================================================================
class Agent : public kernel::Agent_ABC
            , public kernel::Extension_ABC
            , public kernel::Drawable_ABC
            , public kernel::Updatable_ABC< kernel::InstanciationComplete >
            , public kernel::Updatable_ABC< ASN1T_MsgChangeAutomateAck >
            , public kernel::Updatable_ABC< ASN1T_MsgChangeAutomate >
{

public:
    //! @name Constructors/Destructor
    //@{
             Agent( const ASN1T_MsgPionCreation& message,
                    kernel::Controller& controller, 
                    const kernel::Resolver_ABC< kernel::AgentType >& resolver,
                    const kernel::Resolver_ABC< kernel::Automat_ABC >& automatResolver, 
                    const kernel::Resolver_ABC< kernel::KnowledgeGroup_ABC >& gtiaResolver );
    virtual ~Agent();
    //@}

    //! @name Operations
    //@{
    virtual kernel::KnowledgeGroup_ABC& GetKnowledgeGroup() const;
    virtual const kernel::Automat_ABC& GetAutomat() const;

    virtual QString GetName() const;
    virtual unsigned long GetId() const;
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
    virtual void DoUpdate( const kernel::InstanciationComplete& );
    virtual void DoUpdate( const ASN1T_MsgChangeAutomateAck& message );
    virtual void DoUpdate( const ASN1T_MsgChangeAutomate& message );

    void ChangeAutomat( unsigned long id );

    void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;

    void RemoveChild( kernel::Agent_ABC& child );
    void AddChild( kernel::Agent_ABC& child );

    void CreateDictionary();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Resolver_ABC< kernel::Automat_ABC >& automatResolver_;
    const kernel::Resolver_ABC< kernel::KnowledgeGroup_ABC >&  gtiaResolver_;
    unsigned long id_;
    QString   name_;
    kernel::AgentType&   type_;
    kernel::Automat_ABC* automat_;
    bool isPc_;
    //@}
};

#endif // __Agent_h_
