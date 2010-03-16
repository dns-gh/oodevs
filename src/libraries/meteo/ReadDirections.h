// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ReadDirections_h_
#define __ReadDirections_h_


#include "MT_Tools/MT_Vector2D.h"
#include "protocol/Protocol.h"

// =============================================================================
/** @class  ReadDirections
    @brief  ReadDirections
*/
// Created: HBD 2010-03-10
// =============================================================================
class ReadDirections
{

public:
    //! @name Constructors/Destructor
    //@{
    ReadDirections() {};
    virtual ~ReadDirections() {};
    //@}
   
    static void ReadDirection( const Common::MsgHeading& asnDir, MT_Vector2D& vDir )
    {
        vDir = MT_Vector2D( 0, 1 ); // North vector

        MT_Float rRadAngle = asnDir.heading() * MT_PI / 180.;
        vDir.Rotate( rRadAngle );
        vDir.Normalize();
        assert( !vDir.IsZero() );
    }
};

#endif // __ReadDirections_h_
