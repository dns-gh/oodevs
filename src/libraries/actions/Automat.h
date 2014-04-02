// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterAutomat_h_
#define __ActionParameterAutomat_h_

#include "Entity.h"
#include "clients_kernel/Automat_ABC.h"

namespace kernel
{
    class EntityResolver_ABC;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  Automat
    @brief  Automat
*/
// Created: SBO 2007-05-22
// =============================================================================
class Automat : public Entity< kernel::Automat_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             Automat( const kernel::OrderParameter& parameter, kernel::Controller& controller );
             Automat( const kernel::OrderParameter& parameter, const kernel::Automat_ABC& automat, kernel::Controller& controller );
             Automat( const kernel::OrderParameter& parameter, unsigned int id, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller );
    virtual ~Automat();
    //@}

    //! @name Operations
    //@{
    using Entity< kernel::Automat_ABC >::CommitTo;
    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& message ) const;
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual std::string SerializeType() const;
    //@}
};

}
}

#endif // __ActionParameterAutomat_h_
