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

#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/StructuralStateAttribute_ABC.h"
#include "clients_kernel/Units.h"

namespace kernel
{
    class Controllers;
    class UrbanObject_ABC;
}

namespace gui
{
    class PropertiesDictionary;
}

// =============================================================================
/** @class  StructuralStateAttribute
    @brief  StructuralStateAttribute
*/
// Created: JSR 2010-09-01
// =============================================================================
class StructuralStateAttribute : public kernel::StructuralStateAttribute_ABC
                               , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             StructuralStateAttribute( xml::xistream& xis, kernel::UrbanObject_ABC& object, gui::PropertiesDictionary& dictionary );
             StructuralStateAttribute( kernel::UrbanObject_ABC& object, gui::PropertiesDictionary& dictionary );
    virtual ~StructuralStateAttribute();
    //@}

    //! @name Operations
    //@{
    virtual unsigned int GetValue() const;
    void Update( xml::xistream& xis );
    //@}

    //! @name Serializable_ABC
    //@{
    virtual void SerializeAttributes      ( xml::xostream& ) const;
    virtual void SerializeObjectAttributes( xml::xostream& ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void CreateDictionary( kernel::UrbanObject_ABC& object, gui::PropertiesDictionary& dictionary );
    //@}

private:
    //! @name Member data
    //@{
    kernel::UnitedValue< unsigned int > structuralState_;
    //@}
};

#endif // __StructuralStateAttribute_h_
