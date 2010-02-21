// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_Numeric_h_
#define __MissionParameter_Numeric_h_

#include "MissionParameter_ABC.h"

////using namespace Common;


namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_Numeric
    @brief  MissionParameter_Numeric
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_Numeric : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionParameter_Numeric( const Common::MsgMissionParameter& asn );
    virtual ~MissionParameter_Numeric();
    //@}

    //! @name Operations
    //@{
    virtual void Send     ( Common::MsgMissionParameter& asn ) const;
    virtual void Delete( Common::MsgMissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_Numeric( const MissionParameter_Numeric& );            //!< Copy constructor
    MissionParameter_Numeric& operator=( const MissionParameter_Numeric& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    double value_;
    //@}
};

}

#endif // __MissionParameter_Numeric_h_
