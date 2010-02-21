// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_LimasOrder_h_
#define __MissionParameter_LimasOrder_h_

#include "MissionParameter_ABC.h"
#include "tools/Resolver.h"

////using namespace Common;

namespace dispatcher
{
    class LimaOrder;

// =============================================================================
/** @class  MissionParameter_LimasOrder
    @brief  MissionParameter_LimasOrder
*/
// Created: SBO 2008-03-04
// =============================================================================
class MissionParameter_LimasOrder : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MissionParameter_LimasOrder( const Common::MsgMissionParameter& asn );
    virtual ~MissionParameter_LimasOrder();
    //@}

    //! @name Operations
    //@{
    virtual void Send     ( Common::MsgMissionParameter& asn ) const;
    virtual void Delete( Common::MsgMissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_LimasOrder( const MissionParameter_LimasOrder& );            //!< Copy constructor
    MissionParameter_LimasOrder& operator=( const MissionParameter_LimasOrder& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::vector< LimaOrder > limaOrders_;
    //@}
};

}

#endif // __MissionParameter_LimasOrder_h_
