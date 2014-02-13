// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterFormation_h_
#define __ActionParameterFormation_h_

#include "Entity.h"
#include "clients_kernel/Formation_ABC.h"

namespace kernel
{
    class EntityResolver_ABC;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  Formation
    @brief  Formation
*/
// Created: SBO 2007-10-23
// =============================================================================
class Formation : public Entity< kernel::Formation_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             Formation( const kernel::OrderParameter& parameter, kernel::Controller& controller );
             Formation( const kernel::OrderParameter& parameter, const kernel::Formation_ABC& formation, kernel::Controller& controller );
             Formation( const kernel::OrderParameter& parameter, const int& message, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller );
    virtual ~Formation();
    //@}

    //! @name Operations
    //@{
    using Entity< kernel::Formation_ABC >::CommitTo;
    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& message ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual std::string SerializeType() const;
    //@}
};

}
}

#endif // __ActionParameterFormation_h_
