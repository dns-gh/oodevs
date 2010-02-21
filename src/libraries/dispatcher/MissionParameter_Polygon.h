// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_Polygon_h_
#define __MissionParameter_Polygon_h_

#include "MissionParameter_ABC.h"
#include "Localisation.h"

//using namespace Common;

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_Polygon
    @brief  MissionParameter_Polygon
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_Polygon : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionParameter_Polygon( const Common::MsgMissionParameter& asn );
    virtual ~MissionParameter_Polygon();
    //@}

    //! @name Operations
    //@{
    virtual void Send     ( Common::MsgMissionParameter& asn ) const;
    virtual void Delete( Common::MsgMissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_Polygon( const MissionParameter_Polygon& );            //!< Copy constructor
    MissionParameter_Polygon& operator=( const MissionParameter_Polygon& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    Localisation polygon_;
    //@}
};

}

#endif // __MissionParameter_Polygon_h_
