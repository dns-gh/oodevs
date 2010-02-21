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

namespace Common
{
    enum ObstacleType_DemolitionTargetType;

    class MsgObjectAttributes;
}

namespace dispatcher
{
    class Model;

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
     ObstacleAttribute( const Model& model, const Common::MsgObjectAttributes& asnMsg );
    virtual ~ObstacleAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update   ( const Common::MsgObjectAttributes& asnMsg );
    virtual void Send     ( Common::MsgObjectAttributes& asnMsg ) const;
    virtual void Delete( Common::MsgObjectAttributes& asnMsg ) const;
    //@}

private:
    Common::ObstacleType_DemolitionTargetType obstacle_;
};

}

#endif // __ObstacleAttribute_h_
