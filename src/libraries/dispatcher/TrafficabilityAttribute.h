 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TrafficabilityAttribute_h_
#define __TrafficabilityAttribute_h_

#include "ObjectAttribute_ABC.h"

namespace dispatcher
{
// =============================================================================
/** @class  TrafficabilityAttribute
    @brief  TrafficabilityAttribute
*/
// Created: CMA 2011-09-06
// =============================================================================
class TrafficabilityAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit TrafficabilityAttribute( const sword::ObjectAttributes& asnMsg );
    virtual ~TrafficabilityAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const sword::ObjectAttributes& asnMsg );
    virtual void Send  ( sword::ObjectAttributes& asnMsg ) const;
    //@}

private:
    //! @name Member data
    //@{
    double max_;
    //@}
};

}

#endif // __TrafficabilityAttribute_h_
