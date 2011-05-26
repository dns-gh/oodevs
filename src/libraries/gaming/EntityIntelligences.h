// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EntityIntelligences_h_
#define __EntityIntelligences_h_

#include "clients_kernel/EntityHierarchies.h"
#include "clients_kernel/IntelligenceHierarchies.h"
#include "clients_kernel/Updatable_ABC.h"
#include "protocol/Protocol.h"
#include "tools/Resolver_ABC.h"

namespace kernel
{
    class Controller;
    class Entity_ABC;
    class Intelligence_ABC;
    class Formation_ABC;
    class SymbolFactory;
}

// =============================================================================
/** @class  EntityIntelligences
    @brief  EntityIntelligences
*/
// Created: SBO 2007-10-18
// =============================================================================
class EntityIntelligences : public kernel::EntityHierarchies< kernel::IntelligenceHierarchies >
                          , public kernel::Updatable_ABC< sword::IntelligenceCreation >
{
public:
    //! @name Constructors/Destructor
    //@{
             EntityIntelligences( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior, const tools::Resolver_ABC< kernel::Formation_ABC >& resolver );
             EntityIntelligences( kernel::Controller& controller, kernel::Intelligence_ABC& holder, const tools::Resolver_ABC< kernel::Formation_ABC >& resolver, kernel::SymbolFactory& factory );
    virtual ~EntityIntelligences();
    //@}

    //! @name Operations
    //@{
    void ChangeSuperior( kernel::Entity_ABC& superior );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    EntityIntelligences( const EntityIntelligences& );            //!< Copy constructor
    EntityIntelligences& operator=( const EntityIntelligences& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::IntelligenceCreation& message );
    virtual std::string GetSymbol() const;
    virtual std::string GetLevel() const;
    virtual void UpdateSymbol( bool up = true );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Intelligence_ABC* holder_;
    const tools::Resolver_ABC< kernel::Formation_ABC >& resolver_;
    const std::string holderLevelSymbol_;
    //@}
};

#endif // __EntityIntelligences_h_
