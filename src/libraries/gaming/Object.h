// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Object_h_
#define __Object_h_

#include "clients_gui/EntityImplementation.h"
#include "clients_kernel/Types.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/OptionalValue.h"
#include <tools/Resolver_ABC.h>

namespace sword
{
    class ObjectCreation;
    class ObjectUpdate;
}

namespace kernel
{
    class Controller;
    class ObjectType;
    class DotationType;
    class Displayer_ABC;
    class CoordinateConverter_ABC;
}

// =============================================================================
// Created: SBO 2005-09-02
// =============================================================================
class Object : public gui::EntityImplementation< kernel::Object_ABC >
             , public kernel::Extension_ABC
             , public kernel::Updatable_ABC< sword::ObjectUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
             Object( const sword::ObjectCreation& message,
                     kernel::Controller& controller,
                     const kernel::CoordinateConverter_ABC& converter,
                     const tools::Resolver_ABC< kernel::ObjectType, std::string >& typeResolver,
                     const T_CanBeRenamedFunctor& canBeRenamedFunctor );
    virtual ~Object();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    //@}

    //! @name Accessors
    //@{
    const kernel::ObjectType& GetType() const;
    virtual std::string GetSymbol() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Object( const Object& );            //!< Copy constructor
    Object& operator=( const Object& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::ObjectUpdate& message );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    const kernel::ObjectType& type_;
    //@}
};

#endif // __Object_h_
