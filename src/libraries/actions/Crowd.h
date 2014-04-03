// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Crowd_h_
#define __Crowd_h_

#include "Entity.h"
#include "clients_kernel/Population_ABC.h"

namespace kernel
{
    class EntityResolver_ABC;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  Crowd
    @brief  Crowd
*/
// Created: FPO 2011-05-30
// =============================================================================
class Crowd : public Entity< kernel::Population_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             Crowd( const kernel::OrderParameter& parameter, kernel::Controller& controller );
             Crowd( const kernel::OrderParameter& parameter, unsigned int id, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller );
    virtual ~Crowd();
    //@}

    //! @name Operations
    //@{
    virtual std::string SerializeType() const;
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Crowd( const Crowd& );            //!< Copy constructor
    Crowd& operator=( const Crowd& ); //!< Assignment operator
    //@}
};

    }
}

#endif // __Crowd_h_
