// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __StructuralStateAttribute_h_
#define __StructuralStateAttribute_h_

#include "Overridable_ABC.h"
#include "clients_kernel/ModesObserver_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/StructuralStateAttribute_ABC.h"
#include "tools/Observer_ABC.h"

namespace kernel
{
    class Controllers;
    class PropertiesDictionary;
    class UrbanObject_ABC;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  StructuralStateAttribute
    @brief  StructuralStateAttribute
*/
// Created: JSR 2010-09-01
// =============================================================================
class StructuralStateAttribute : public kernel::StructuralStateAttribute_ABC
                               , public Overridable_ABC
                               , public tools::Observer_ABC
                               , public kernel::ModesObserver_ABC
                               , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             StructuralStateAttribute( kernel::Controllers& controllers, kernel::UrbanObject_ABC& object, kernel::PropertiesDictionary& dictionary );
    virtual ~StructuralStateAttribute();
    //@}

    //! @name Operations
    //@{
    virtual bool IsOverriden() const;
    virtual unsigned int GetValue() const;
    void Update( xml::xistream& xis );
    //@}

    //! @name Serializable_ABC
    //@{
    virtual void SerializeAttributes( xml::xostream& ) const;
    //@}

    //! @name ModesObserver_ABC
    //@{
    virtual void NotifyModeChanged( int newMode );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers&          controllers_;
    kernel::UrbanObject_ABC&      object_;
    unsigned int                  structuralState_;
    kernel::PropertiesDictionary& dictionary_;
    //@}
};

#endif // __StructuralStateAttribute_h_
