// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __MIL_Effect_DetectionRange_h_
#define __MIL_Effect_DetectionRange_h_

#include "MIL_Effect_ABC.h"
#include "protocol/ClientSenders.h"

// =============================================================================
/** @class  MIL_Effect_DetectionRange
    @brief  MIL_Effect_DetectionRange
*/
// Created: JSR 2013-04-24
// =============================================================================
class MIL_Effect_DetectionRange : public MIL_Effect_ABC
{
private:
    //! @name Types
    //@{
    typedef const std::vector< unsigned int > T_IdVector;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             MIL_Effect_DetectionRange( T_IdVector& perceiversIds, int resourceId, T_IdVector& fireEffectsIds );
    virtual ~MIL_Effect_DetectionRange();
    //@}

    //! @name Operations
    //@{
    virtual bool Execute();
    //@}

private:
    //! @name Member data
    //@{
    client::IndirectFirePerception msg_;
    //@}
};

#endif // __MIL_Effect_DetectionRange_h_
