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
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class Controller;
    class Entity_ABC;
    class Intelligence_ABC;
    class SymbolFactory;
}

// =============================================================================
/** @class  EntityIntelligences
    @brief  EntityIntelligences
*/
// Created: SBO 2007-10-18
// =============================================================================
class EntityIntelligences : public kernel::EntityHierarchies< kernel::IntelligenceHierarchies >
                          , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             EntityIntelligences( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior );
             EntityIntelligences( kernel::Controller& controller, kernel::Intelligence_ABC& holder, kernel::Entity_ABC* superior, kernel::SymbolFactory& factory );
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
    virtual std::string GetSymbol() const;
    virtual std::string GetLevel() const;
    virtual void UpdateSymbol( bool up = true );
    virtual void SerializeIntelligences( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Intelligence_ABC* holder_;
    const std::string holderLevelSymbol_;
    //@}
};

#endif // __EntityIntelligences_h_
