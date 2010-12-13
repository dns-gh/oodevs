// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __GenObject_h_
#define __GenObject_h_

#include "Localisation.h"

namespace sword
{
    enum ObstacleType_DemolitionTargetType;
    class MsgPlannedWork;
}

namespace dispatcher
{

// =============================================================================
/** @class  GenObject
    @brief  Gen object
*/
// Created: NLD 2007-04-23
// =============================================================================
class GenObject
{
public:
    //! @name Constructors/Destructor
    //@{
             GenObject();
    explicit GenObject( const sword::MsgPlannedWork& asn );
    virtual ~GenObject();
    //@}

    //! @name Operations
    //@{
    void Send( sword::MsgPlannedWork& asn ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::string type_;
    Localisation location_;
    sword::ObstacleType_DemolitionTargetType typeObstacle_;
    float density_;
    unsigned int tc2_;
    unsigned int delaiActiviteMines_;
    //@}
};

}

#endif // __GenObject_h_
