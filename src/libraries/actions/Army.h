// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Army_h_
#define __Army_h_

#include "Entity.h"
#include "clients_kernel/Team_ABC.h"

namespace kernel
{
    class EntityResolver_ABC;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  Army
    @brief  Army
*/
// Created: JSR 2010-04-14
// =============================================================================
class Army : public Entity< kernel::Team_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             Army( const kernel::OrderParameter& parameter, kernel::Controller& controller );
             Army( const kernel::OrderParameter& parameter, unsigned int id, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller );
             Army( const kernel::OrderParameter& parameter, const kernel::Team_ABC& team, kernel::Controller& controller );
    virtual ~Army();
    //@}

    //! @name Operations
    //@{
    using Entity< kernel::Team_ABC >::CommitTo;
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

#endif // __Army_h_
