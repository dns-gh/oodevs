// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UrbanKnowledge_h_
#define __UrbanKnowledge_h_

#include "game_asn/Simulation.h"
#include "clients_kernel/UrbanKnowledge_ABC.h"
#include "tools/Resolver_ABC.h"
#include "clients_kernel/OptionalValue.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Displayable_ABC.h"
#include "clients_kernel/EntityImplementation.h"

namespace kernel
{
    class Displayer_ABC;
    class Controller;
    class Agent_ABC;
}

namespace urban
{
    class Block;
}

// =============================================================================
/** @class  UrbanKnowledge
    @brief  UrbanKnowledge
*/
// Created: MGD 2009-12-10
// =============================================================================
class UrbanKnowledge : public kernel::EntityImplementation< kernel::UrbanKnowledge_ABC >
                      , public kernel::Extension_ABC
                      , public kernel::Updatable_ABC< ASN1T_MsgUrbanKnowledgeUpdate >
                      , public kernel::Displayable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanKnowledge( const kernel::Team_ABC& owner, const ASN1T_MsgUrbanKnowledgeCreation& message,
                              kernel::Controller& controller,
                              const tools::Resolver_ABC< urban::Block >& terrainObjectResolver );
    virtual ~UrbanKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual QString GetName() const;
    virtual QString GetTypeName() const;
    virtual const kernel::Entity_ABC* GetEntity() const;
    virtual const kernel::Entity_ABC* GetRecognizedEntity() const;
    virtual const kernel::Team_ABC&   GetOwner() const;

    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInList( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    UrbanKnowledge( const UrbanKnowledge& );            //!< Copy constructor
    UrbanKnowledge& operator=( const UrbanKnowledge& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgUrbanKnowledgeUpdate& message );
    //@}

private:
    //! @name Member data
    //@{
    const tools::Resolver_ABC< urban::Block >& terrainObjectResolver_;
    const kernel::Team_ABC& owner_;

    urban::TerrainObject_ABC* pRealUrban_;

    kernel::OptionalValue< bool >           bIsPerceived_;
    kernel::OptionalValue< unsigned int >   nRelevance_;
    kernel::OptionalValue< kernel::E_PerceptionResult > nCurrrentPerceptionLevel_;
    kernel::OptionalValue< unsigned int > nProgress_;
    //@}
};

#endif // __UrbanKnowledge_h_
