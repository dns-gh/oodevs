// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Equipment_h_
#define __Equipment_h_

#include "game_asn/Asn.h"

namespace kernel
{
    class EquipmentType;
}

// =============================================================================
/** @class  Equipment
    @brief  Equipment
*/
// Created: AGE 2006-02-21
// =============================================================================
class Equipment
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Equipment( const kernel::EquipmentType& type );
             Equipment( const Equipment& rhs );
    virtual ~Equipment();
    //@}

    //! @name Operations
    //@{
    void Update( const ASN1T_DotationEquipement& message );
    QString GetName() const;
    unsigned Total() const;
    //@}

    //! @name Operators
    //@{
    Equipment operator+( const Equipment& rhs ) const;
    Equipment operator-( const Equipment& rhs ) const;
    Equipment& operator=( const Equipment& rhs );
    //@}

public:
    //! @name Member data
    //@{
    const kernel::EquipmentType& type_;

    int available_;
    int unavailable_;
    int repairable_;
    int inMaintenance_;
    int prisonners_;
    //@}
};

#endif // __Equipment_h_
