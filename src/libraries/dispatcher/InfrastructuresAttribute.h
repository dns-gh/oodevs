// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __InfrastructuresAttribute_h_
#define __InfrastructuresAttribute_h_

#include "UrbanObjectAttribute_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  InfrastructuresAttribute
    @brief  InfrastructuresAttribute
*/
// Created: JSR 2010-08-17
// =============================================================================
class InfrastructuresAttribute : public UrbanObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit InfrastructuresAttribute( const sword::UrbanAttributes& message );
    virtual ~InfrastructuresAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const sword::UrbanAttributes& message );
    virtual void Send  (       sword::UrbanAttributes& message ) const;
    //@}
};

}

#endif // __InfrastructuresAttribute_h_
