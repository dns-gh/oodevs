// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Inhabitant_h_
#define __Inhabitant_h_

#include "Entity.h"
#include "clients_kernel/Inhabitant_ABC.h"

namespace kernel
{
    class EntityResolver_ABC;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  Inhabitant
    @brief  Inhabitant
*/
// Created: FPO 2011-05-30
// =============================================================================
class Inhabitant : public Entity< kernel::Inhabitant_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             Inhabitant( const kernel::OrderParameter& parameter, kernel::Controller& controller );
             Inhabitant( const kernel::OrderParameter& parameter, unsigned int id, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller );
    virtual ~Inhabitant();
    //@}

    //! @name Operations
    //@{
    virtual std::string SerializeType() const;
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Inhabitant( const Inhabitant& );            //!< Copy constructor
    Inhabitant& operator=( const Inhabitant& ); //!< Assignment operator
    //@}
};

    }
}

#endif // __Inhabitant_h_
