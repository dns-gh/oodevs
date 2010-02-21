// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_Point_h_
#define __MissionParameter_Point_h_

#include "MissionParameter_ABC.h"
#include "Localisation.h"

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_Point
    @brief  MissionParameter_Point
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_Point : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MissionParameter_Point( const Common::MsgMissionParameter& asn );
    virtual ~MissionParameter_Point();
    //@}

    //! @name Operations
    //@{
    virtual void Send  ( Common::MsgMissionParameter& asn ) const;
    virtual void Delete( Common::MsgMissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_Point( const MissionParameter_Point& );            //!< Copy constructor
    MissionParameter_Point& operator=( const MissionParameter_Point& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    Localisation point_;
    //@}
};

}

#endif // __MissionParameter_Point_h_
