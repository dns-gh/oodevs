// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_EquipmentType_h_
#define __MissionParameter_EquipmentType_h_

#include "MissionParameter_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_EquipmentType
    @brief  MissionParameter_EquipmentType
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_EquipmentType : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionParameter_EquipmentType( const ASN1T_MissionParameter& asn );
    virtual ~MissionParameter_EquipmentType();
    //@}

    //! @name Operations
    //@{
    virtual void Send     ( ASN1T_MissionParameter& asn ) const;
    virtual void AsnDelete( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_EquipmentType( const MissionParameter_EquipmentType& );            //!< Copy constructor
    MissionParameter_EquipmentType& operator=( const MissionParameter_EquipmentType& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned int equipmentType_;
    //@}
};

}

#endif // __MissionParameter_EquipmentType_h_
