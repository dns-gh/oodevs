// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_Fire_h_
#define __MIL_Fire_h_

#include "MIL.h"
#include "MIL_RealObject_ABC.h"

// =============================================================================
/** @class  MIL_Fire
    @brief  MIL_Fire    
*/
// Created: JCR 2007-09-19
// =============================================================================
class MIL_Fire : public MIL_RealObject_ABC
{
    MT_COPYNOTALLOWED( MIL_Fire );

public:
    //! @name Constructors/Destructor
    //@{
            MIL_Fire( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
            MIL_Fire();
    virtual ~MIL_Fire();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
    //@}

private:
    //! @name Events
    //@{
    virtual void ProcessAgentInside  ( MIL_Agent_ABC& agent );
    virtual void ProcessAgentEntering( MIL_Agent_ABC& agent );
    //@}

};

#endif // __MIL_Fire_h_
