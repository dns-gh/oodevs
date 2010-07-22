// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CapacityAttribute_h_
#define __CapacityAttribute_h_

#include "UrbanObjectAttribute_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  CapacityAttribute
    @brief  CapacityAttribute
*/
// Created: SLG 2009-12-04
// =============================================================================
class CapacityAttribute : public UrbanObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit CapacityAttribute( const MsgsSimToClient::MsgUrbanAttributes& message );
    virtual ~CapacityAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const MsgsSimToClient::MsgUrbanAttributes& message );
    virtual void Send  (       MsgsSimToClient::MsgUrbanAttributes& message ) const;
    //@}

private:
    //! @name Data members
    //@{
    float structuralState_;
    //@}
};

}

#endif // __CapacityAttribute_h_
