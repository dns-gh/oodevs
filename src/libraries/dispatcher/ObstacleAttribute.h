 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObstacleAttribute_h_
#define __ObstacleAttribute_h_

#include "ObjectAttribute_ABC.h"

namespace sword
{
    enum ObstacleType_DemolitionTargetType;
}

namespace dispatcher
{
// =============================================================================
/** @class  ObstacleAttribute
    @brief  ObstacleAttribute
*/
// Created: NLD 2006-09-19
// =============================================================================
class ObstacleAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ObstacleAttribute( const sword::ObjectAttributes& message );
    virtual ~ObstacleAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const sword::ObjectAttributes& message );
    virtual void Send  ( sword::ObjectAttributes& message ) const;
    //@}

private:
    //! @name Member data
    //@{
    sword::ObstacleType_DemolitionTargetType obstacle_;
    bool isActivated_;
    int activationTime_;
    int activityTime_;
    //@}
};

}

#endif // __ObstacleAttribute_h_
