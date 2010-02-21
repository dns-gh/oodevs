// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_Objective_h_
#define __MissionParameter_Objective_h_

#include "MissionParameter_ABC.h"
#include "Objective.h"

////using namespace Common;

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_Objective
    @brief  MissionParameter_Objective
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_Objective : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionParameter_Objective( const Common::MsgMissionParameter& asn );
    virtual ~MissionParameter_Objective();
    //@}

    //! @name Operations
    //@{
    virtual void Send     ( Common::MsgMissionParameter& asn ) const;
    virtual void Delete( Common::MsgMissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_Objective( const MissionParameter_Objective& );            //!< Copy constructor
    MissionParameter_Objective& operator=( const MissionParameter_Objective& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    Objective data_;
    //@}
};

}

#endif // __MissionParameter_Objective_h_
