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
#include "protocol/Protocol.h"

namespace dispatcher
{

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
    explicit ToxicCloudAttribute( const sword::ObjectAttributes& message );
    virtual ~ToxicCloudAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const sword::ObjectAttributes& message );
    virtual void Send  ( sword::ObjectAttributes& message ) const;
    //@}
private:
    //! @name
    //@{
    void Clear();
    //@}

private:
    //! @name Data member
    //@{
    sword::LocatedQuantityList quantities_;
    //@}
};

}

#endif // __ToxicCloudAttribute_h_
