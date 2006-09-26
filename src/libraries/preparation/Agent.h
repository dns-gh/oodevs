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

#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/DataDictionary.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class Controller;
    class AgentType;
    class AutomatType;
    class Formation_ABC;
}

namespace xml
{
    class xostream;
}

class IdManager;

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
            , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Agent( const kernel::Formation_ABC& parent, const kernel::AutomatType& type, kernel::Controller& controller, IdManager& idManager );
             Agent( const kernel::Agent_ABC& parent, const kernel::AgentType& type, kernel::Controller& controller, IdManager& idManager );
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
    virtual void DoSerialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Agent( const Agent& );            //!< Copy constructor
    Agent& operator=( const Agent& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void Aggregate( const bool& );
    void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;
    void CreateDictionary();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    unsigned long id_;
    QString   name_;

    const kernel::Formation_ABC* formation_;
    const kernel::Agent_ABC*     automat_;
    const kernel::AutomatType*   automatType_;
    const kernel::AgentType*     type_;

    bool aggregated_;
    //@}
};

#endif // __Agent_h_
