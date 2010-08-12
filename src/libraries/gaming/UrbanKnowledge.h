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
    class TerrainObject_ABC;
}

namespace MsgsSimToClient
{
    class MsgUrbanKnowledgeUpdate;
    class MsgUrbanKnowledgeCreation;
}

// =============================================================================
/** @class  UrbanKnowledge
    @brief  UrbanKnowledge
*/
// Created: MGD 2009-12-10
// =============================================================================
class UrbanKnowledge : public kernel::EntityImplementation< kernel::UrbanKnowledge_ABC >
                     , public kernel::Extension_ABC
                     , public kernel::Updatable_ABC< MsgsSimToClient::MsgUrbanKnowledgeUpdate >
                     , public kernel::Displayable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanKnowledge( const kernel::Team_ABC& owner, const MsgsSimToClient::MsgUrbanKnowledgeCreation& message,
                             kernel::Controller& controller,
                             const tools::Resolver_ABC< urban::TerrainObject_ABC >& terrainObjectResolver );
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
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UrbanKnowledge( const UrbanKnowledge& );            //!< Copy constructor
    UrbanKnowledge& operator=( const UrbanKnowledge& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const MsgsSimToClient::MsgUrbanKnowledgeUpdate& message );
    //@}

private:
    //! @name Member data
    //@{
    const tools::Resolver_ABC< urban::TerrainObject_ABC >& terrainObjectResolver_;
    const kernel::Team_ABC& owner_;

    urban::TerrainObject_ABC* pRealUrban_;

    kernel::OptionalValue< bool > bIsPerceived_;
    kernel::OptionalValue< float > rProgress_;
    kernel::OptionalValue< float > rMaxProgress_;
    //@}
};

#endif // __UrbanKnowledge_h_
