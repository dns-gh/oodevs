// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectAttributePrototypeFactory_ABC_h_
#define __ObjectAttributePrototypeFactory_ABC_h_

#include "tools/Resolver_ABC.h"

class QWidget;

namespace xml
{
    class xistream;
}

namespace gui
{
    class ObjectAttributePrototype_ABC;

// =============================================================================
/** @class  ObjectAttributesPrototypeContainer
    @brief  Object prototype base class
*/
// Created: SBO 2006-04-18
// =============================================================================
class ObjectAttributePrototypeFactory_ABC
{
public:
    //! @name
    //@{
    typedef std::vector< ObjectAttributePrototype_ABC* > T_AttributeContainer;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             ObjectAttributePrototypeFactory_ABC() {}
    virtual ~ObjectAttributePrototypeFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Create( const std::string& capacity, xml::xistream& xis, T_AttributeContainer& container, QWidget* parent ) const = 0;
    virtual void FinalizeCreate() const = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectAttributePrototypeFactory_ABC( const ObjectAttributePrototypeFactory_ABC& );            //!< Copy constructor
    ObjectAttributePrototypeFactory_ABC& operator=( const ObjectAttributePrototypeFactory_ABC& ); //!< Assignment operator
    //@}
};

}

#endif // __ObjectAttributePrototypeFactory_ABC_h_
