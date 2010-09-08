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

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class PropertiesDictionary;
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
                               , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             StructuralStateAttribute( unsigned int value, kernel::PropertiesDictionary& dico );
    virtual ~StructuralStateAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    void Update( xml::xistream& xis );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    StructuralStateAttribute( const StructuralStateAttribute& );            //!< Copy constructor
    StructuralStateAttribute& operator=( const StructuralStateAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dico );
    //@}

private:
    //! @name Member data
    //@{
    unsigned int structuralState_;
    //@}
};

#endif // __StructuralStateAttribute_h_
