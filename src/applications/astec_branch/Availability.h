// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Availability_h_
#define __Availability_h_

class EquipmentType;

// =============================================================================
/** @class  Availability
    @brief  Availability
*/
// Created: AGE 2006-02-28
// =============================================================================
class Availability
{

public:
    //! @name Constructors/Destructor
    //@{
             Availability();
             Availability( const EquipmentType& type, unsigned int available );
    virtual ~Availability();
    //@}

    //! @name Operations
    //@{
    //@}

public:
    //! @name Member data
    //@{
    const EquipmentType* type_;
    unsigned int available_;
    //@}
};

#endif // __Availability_h_
