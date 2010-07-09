// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_Direction_h_
#define __MissionParameter_Direction_h_

#include "MissionParameter_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_Direction
    @brief  MissionParameter_Direction
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_Direction : public MissionParameter_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MissionParameter_Direction( const Common::MsgMissionParameter& asn );
    virtual ~MissionParameter_Direction();
    //@}

    //! @name Operations
    //@{
    virtual void Send( Common::MsgMissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_Direction( const MissionParameter_Direction& );            //!< Copy constructor
    MissionParameter_Direction& operator=( const MissionParameter_Direction& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned int direction_;
    //@}
};

}

#endif // __MissionParameter_Direction_h_
