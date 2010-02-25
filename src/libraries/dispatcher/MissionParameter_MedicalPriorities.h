// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_MedicalPriorities_h_
#define __MissionParameter_MedicalPriorities_h_

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
    explicit MissionParameter_MedicalPriorities( const Common::MsgMissionParameter& asn );
    virtual ~MissionParameter_MedicalPriorities();
    //@}

    //! @name Operations
    //@{
    virtual void Send( Common::MsgMissionParameter& asn ) const;
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
    std::vector< int > medicalPriorities_;
    //@}
};

}

#endif // __MissionParameter_MedicalPriorities_h_
