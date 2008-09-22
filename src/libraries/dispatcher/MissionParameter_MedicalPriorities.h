// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_MedicalPriorities_h_
#define __MissionParameter_MedicalPriorities_h_

#include "game_asn/Simulation.h"
#include "MissionParameter_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_MedicalPriorities
    @brief  MissionParameter_MedicalPriorities
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_MedicalPriorities : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MissionParameter_MedicalPriorities( const ASN1T_MissionParameter& asn );
    virtual ~MissionParameter_MedicalPriorities();
    //@}

    //! @name Operations
    //@{
    virtual void Send     ( ASN1T_MissionParameter& asn ) const;
    virtual void AsnDelete( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_MedicalPriorities( const MissionParameter_MedicalPriorities& );            //!< Copy constructor
    MissionParameter_MedicalPriorities& operator=( const MissionParameter_MedicalPriorities& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::vector< ASN1T_EnumHumanWound > medicalPriorities_;
    //@}
};

}

#endif // __MissionParameter_MedicalPriorities_h_
