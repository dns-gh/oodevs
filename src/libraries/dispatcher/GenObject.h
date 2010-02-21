// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GenObject_h_
#define __GenObject_h_


#include "Localisation.h"

namespace Common
{
    enum ObstacleType_DemolitionTargetType;

    class MsgPlannedWork;
}

namespace dispatcher
{

// =============================================================================
/** @class  GenObject
    @brief  GenObject
*/
// Created: NLD 2007-04-23
// =============================================================================
class GenObject
{
   
public:
    //! @name Constructors/Destructor
    //@{
    GenObject( const Common::MsgPlannedWork& asn );
    GenObject();
   virtual ~GenObject();
    //@}

    //! @name Operations
    //@{
   void Send( Common::MsgPlannedWork& asn ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::string                                 type_;
    Localisation                                location_;
    Common::ObstacleType_DemolitionTargetType   typeObstacle_;
    double                                      density_;
    unsigned int                                tc2_;
    unsigned int                                delaiActiviteMines_;
    //@}
};

}

#endif // __GenObject_h_
