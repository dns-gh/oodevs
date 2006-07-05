// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Lend_h_
#define __Lend_h_

class EquipmentType;
class Agent;

// =============================================================================
/** @class  Lend
    @brief  Lend
*/
// Created: AGE 2006-02-21
// =============================================================================
class Lend
{

public:
    //! @name Constructors/Destructor
    //@{
             Lend( const EquipmentType& type, const Agent& borrower, unsigned int quantity );
    virtual ~Lend();
    //@}

    //! @name Operations
    //@{
    //@}

public:
    //! @name Member data
    //@{
    const EquipmentType* type_;
    const Agent* borrower_;
    unsigned int quantity_;
    //@}
};

#endif // __Lend_h_
