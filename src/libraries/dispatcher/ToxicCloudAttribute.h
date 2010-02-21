 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ToxicCloudAttribute_h_
#define __ToxicCloudAttribute_h_

#include "ObjectAttribute_ABC.h"

namespace Common
{
    class LocatedQuantityList;
    class MsgObjectAttributes;
}

namespace dispatcher
{
    class Model;

// =============================================================================
/** @class  ToxicCloudAttribute
    @brief  ToxicCloudAttribute
*/
// Created: NLD 2006-09-19
// =============================================================================
class ToxicCloudAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
     ToxicCloudAttribute( const Model& model, const Common::MsgObjectAttributes& asnMsg );
    virtual ~ToxicCloudAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update   ( const Common::MsgObjectAttributes& asnMsg );
    virtual void Send     ( Common::MsgObjectAttributes& asnMsg ) const;
    virtual void Delete( Common::MsgObjectAttributes& asnMsg ) const;
    //@}
private:
    //! @name 
    //@{
    void Clear();
    //@}

private:
    //! @name Data member
    //@{
    Common::MsgLocatedQuantityList  quantities_;
    //@}
};

}

#endif // __ToxicCloudAttribute_h_
