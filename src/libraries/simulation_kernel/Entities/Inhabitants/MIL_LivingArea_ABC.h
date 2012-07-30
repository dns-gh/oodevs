// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __MIL_LivingArea_ABC_h
#define __MIL_LivingArea_ABC_h

#include "MT_Tools/Mt_Vector2DTypes.h"
#include <boost/serialization/export.hpp>

// =============================================================================
/** @class  MIL_LivingArea_ABC
    @brief  MIL living area declaration
*/
// Created: LGY 2011-01-20
// =============================================================================
class MIL_LivingArea_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_LivingArea_ABC() {}
    virtual ~MIL_LivingArea_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void StartMotivation( const std::string& motivation ) = 0;
    virtual void MovePeople( const std::string& motivation, int occurence ) = 0;
    virtual void FinishMoving( const std::string& motivation ) = 0;
    virtual T_PointVector ComputeMovingArea() const = 0;
    //@}

    //! @name Serialization
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int )
    {
        // NOTHING
    }
    //@}
};

#endif // __MIL_LivingArea_ABC_h
