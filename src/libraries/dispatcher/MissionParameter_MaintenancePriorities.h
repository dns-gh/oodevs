// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_MaintenancePriorities_h_
#define __MissionParameter_MaintenancePriorities_h_

#include "MissionParameter_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_MaintenancePriorities
    @brief  MissionParameter_MaintenancePriorities
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_MaintenancePriorities : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MissionParameter_MaintenancePriorities( const ASN1T_MissionParameter& asn );
    virtual ~MissionParameter_MaintenancePriorities();
    //@}

    //! @name Operations
    //@{
    virtual void Send     ( ASN1T_MissionParameter& asn ) const;
    virtual void AsnDelete( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_MaintenancePriorities( const MissionParameter_MaintenancePriorities& );            //!< Copy constructor
    MissionParameter_MaintenancePriorities& operator=( const MissionParameter_MaintenancePriorities& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::vector< int > maintenancePriorities_;
    //@}
};

}

#endif // __MissionParameter_MaintenancePriorities_h_
