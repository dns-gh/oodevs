// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterPopulationKnowledge_h_
#define __ActionParameterPopulationKnowledge_h_

#include "Knowledge_ABC.h"
#include "clients_kernel/Population_ABC.h"

namespace kernel
{
    class EntityResolver_ABC;
    class Population_ABC;
    class Controller;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  PopulationKnowledge
    @brief  PopulationKnowledge
*/
// Created: SBO 2007-05-24
// =============================================================================
class PopulationKnowledge : public Entity< kernel::Population_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationKnowledge( const kernel::OrderParameter& parameter, kernel::Controller& controller );
             PopulationKnowledge( const kernel::OrderParameter& parameter, unsigned int id,
                                  const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller );
    virtual ~PopulationKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& asn ) const;
    //@}

    //! @name Helpers
    //@{
    virtual std::string SerializeType() const;
    //@}
};

    }
}

#endif // __ActionParameterPopulationKnowledge_h_
