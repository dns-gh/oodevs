// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_Location_h_
#define __MissionParameter_Location_h_

#include "MissionParameter_ABC.h"
#include "Localisation.h"

////using namespace Common;

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_Location
    @brief  MissionParameter_Location
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_Location : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionParameter_Location( const Common::MsgMissionParameter& asn );
    virtual ~MissionParameter_Location();
    //@}

    //! @name Operations
    //@{
    virtual void Send     ( Common::MsgMissionParameter& asn ) const;
    virtual void Delete( Common::MsgMissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_Location( const MissionParameter_Location& );            //!< Copy constructor
    MissionParameter_Location& operator=( const MissionParameter_Location& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    Localisation location_;
    //@}
};

}

#endif // __MissionParameter_Location_h_
