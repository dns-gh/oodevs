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

#include "tools/Resolver.h"
#include "clients_kernel/UrbanKnowledge_ABC.h"
#include "clients_kernel/OptionalValue.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Displayable_ABC.h"
#include "clients_kernel/EntityImplementation.h"

namespace kernel
{
    class Displayer_ABC;
    class Controller;
}

namespace sword
{
    class UrbanKnowledgeUpdate;
    class UrbanKnowledgeCreation;
}

namespace gui
{
    class TerrainObjectProxy;
}

// =============================================================================
/** @class  UrbanKnowledge
    @brief  UrbanKnowledge
*/
// Created: MGD 2009-12-10
// =============================================================================
class UrbanKnowledge : public kernel::EntityImplementation< kernel::UrbanKnowledge_ABC >
                     , public kernel::Extension_ABC
                     , public kernel::Updatable_ABC< sword::UrbanKnowledgeUpdate >
                     , public kernel::Displayable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanKnowledge( const kernel::Team_ABC& owner, const sword::UrbanKnowledgeCreation& message,
                             kernel::Controller& controller,
                             const tools::Resolver< gui::TerrainObjectProxy >& terrainObjectResolver );
    virtual ~UrbanKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual QString GetName() const;
    virtual const kernel::Entity_ABC* GetEntity() const;
    virtual unsigned long GetEntityId() const;
    virtual const kernel::Team_ABC* GetTeam() const;
    virtual const kernel::Team_ABC& GetOwner() const;

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
    virtual void DoUpdate( const sword::UrbanKnowledgeUpdate& message );
    //@}

private:
    //! @name Member data
    //@{
    const tools::Resolver< gui::TerrainObjectProxy >& terrainObjectResolver_;
    const kernel::Team_ABC& owner_;
    gui::TerrainObjectProxy* pRealUrban_;
    unsigned long entityId_;
    kernel::OptionalValue< bool > bIsPerceived_;
    kernel::OptionalValue< int > rProgress_;
    kernel::OptionalValue< int > rMaxProgress_;
    //@}
};

#endif // __UrbanKnowledge_h_
