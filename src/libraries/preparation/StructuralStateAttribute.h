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

// =============================================================================
/** @class  StructuralStateAttribute
    @brief  StructuralStateAttribute
*/
// Created: JSR 2010-09-01
// =============================================================================
class StructuralStateAttribute : public kernel::StructuralStateAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             StructuralStateAttribute( unsigned int value );
    virtual ~StructuralStateAttribute();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    StructuralStateAttribute( const StructuralStateAttribute& );            //!< Copy constructor
    StructuralStateAttribute& operator=( const StructuralStateAttribute& ); //!< Assignment operator
    //@}

public:
    //! @name Member data
    //@{
    unsigned int structuralState_;// TODO private et serialize
    //@}
};

#endif // __StructuralStateAttribute_h_
