// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __UrbanBlockCollisionNotificationHandler_ABC_h_
#define __UrbanBlockCollisionNotificationHandler_ABC_h_

namespace urban
{
    class TerrainObject_ABC;
}

namespace terrain
{

// =============================================================================
/** @class  UrbanBlockCollisionNotificationHandler_ABC
    @brief  UrbanBlockCollisionNotificationHandler_ABC
*/
// Created: SLG 2010-04-09
// =============================================================================
class UrbanBlockCollisionNotificationHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanBlockCollisionNotificationHandler_ABC(){}
    virtual ~UrbanBlockCollisionNotificationHandler_ABC(){}
    //@}

    //! @name Operations
    //@{
    virtual void NotifyMovingInsideUrbanBlock( const urban::TerrainObject_ABC& object ) = 0;
    virtual void NotifyMovingOutsideUrbanBlock( const urban::TerrainObject_ABC& object ) = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UrbanBlockCollisionNotificationHandler_ABC( const UrbanBlockCollisionNotificationHandler_ABC& );            //!< Copy constructor
    UrbanBlockCollisionNotificationHandler_ABC& operator=( const UrbanBlockCollisionNotificationHandler_ABC& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

}

#endif // __UrbanBlockCollisionHandler_ABC_h_
