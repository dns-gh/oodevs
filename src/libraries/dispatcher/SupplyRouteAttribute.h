 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SupplyRouteAttribute_h_
#define __SupplyRouteAttribute_h_

#include "ObjectAttribute_ABC.h"


namespace Common
{
    class MsgObjectAttributes;
}


namespace dispatcher
{
    class Model;

// =============================================================================
/** @class  SupplyRouteAttribute
    @brief  SupplyRouteAttribute
*/
// Created: NLD 2006-09-19
// =============================================================================
class SupplyRouteAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
     SupplyRouteAttribute( const Model& model, const Common::MsgObjectAttributes& asnMsg );
    virtual ~SupplyRouteAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update   ( const Common::MsgObjectAttributes& asnMsg );
    virtual void Send     ( Common::MsgObjectAttributes& asnMsg ) const;
    virtual void Delete( Common::MsgObjectAttributes& asnMsg ) const;
    //@}

private:
    bool         bEquipped_;
    unsigned int nMaxWeight_;
    unsigned int nWidth_;
    unsigned int nLength_;
    unsigned int nFlow_;
};

}

#endif // __SupplyRouteAttribute_h_
