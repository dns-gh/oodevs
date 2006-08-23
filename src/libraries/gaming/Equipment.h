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

#include "ASN_Types.h"

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
    virtual ~Equipment();
    //@}

    //! @name Operations
    //@{
    void Update( const ASN1T_DotationEquipement& message );
    QString GetName() const;
    unsigned Total() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Equipment( const Equipment& );            //!< Copy constructor
    Equipment& operator=( const Equipment& ); //!< Assignement operator
    //@}

public:
    //! @name Member data
    //@{
    const kernel::EquipmentType& type_;

    unsigned available_;
    unsigned unavailable_;
    unsigned repairable_;
    unsigned inMaintenance_;
    //@}
};

#endif // __Equipment_h_
