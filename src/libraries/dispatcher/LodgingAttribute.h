// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LodgingAttribute_h_
#define __LodgingAttribute_h_

#include "ObjectAttribute_ABC.h"
#include "tools/Resolver.h"
#include <boost/noncopyable.hpp>

namespace dispatcher
{
// =============================================================================
/** @class  LodgingAttribute
@brief  LodgingAttribute
*/
// Created: MMC 2011-05-04
// =============================================================================
class LodgingAttribute : private boost::noncopyable
                       , public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    LodgingAttribute( const sword::ObjectAttributes& asnMsg );
    virtual ~LodgingAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const sword::ObjectAttributes& asnMsg );
    virtual void Send( sword::ObjectAttributes& asnMsg ) const;
    //@}

private:
    //! @name Member data
    //@{
    unsigned int capacity_;
    //@}
};

}

#endif // __LodgingAttribute_h_
