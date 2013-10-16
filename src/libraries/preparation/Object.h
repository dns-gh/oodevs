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
#include "clients_kernel/Object_ABC.h"
#include <tools/Resolver_ABC.h>

namespace kernel
{
    class Controller;
    class ObjectType;
    class Displayer_ABC;
    class CoordinateConverter_ABC;
}

namespace xml
{
    class xostream;
    class xistream;
}

namespace tools
{
    class IdManager;
}

// =============================================================================
/** @class  Object
    @brief  Object
*/
// Created: SBO 2005-09-02
// =============================================================================
class Object : public gui::EntityImplementation< kernel::Object_ABC >
             , public kernel::Extension_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Object( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter,
                     const kernel::ObjectType& type, const QString& name, tools::IdManager& idManager );
             Object( xml::xistream& xis, kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter,
                     const kernel::ObjectType& type, tools::IdManager& idManager );
    virtual ~Object();
    //@}

    //! @name Accessors
    //@{
    virtual const kernel::ObjectType& GetType() const;
    virtual std::string GetSymbol() const;
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    void SerializeAttributes( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Object( const Object& );
    Object& operator=( const Object& );
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary();
    //@}

public:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    const kernel::ObjectType& type_;
    //@}
};

#endif // __Object_h_
