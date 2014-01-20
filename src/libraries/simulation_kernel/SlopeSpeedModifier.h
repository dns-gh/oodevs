// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __SlopeSpeedModifier_h_
#define __SlopeSpeedModifier_h_

#include <boost/noncopyable.hpp>

class PHY_RawVisionData;
class MT_Vector2D;

// =============================================================================
/** @class  SlopeSpeedModifier
    @brief  SlopeSpeedModifier
*/
// Created: JSR 2013-11-08
// =============================================================================
class SlopeSpeedModifier : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::vector< std::pair< MT_Vector2D, double > > T_Slopes;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             SlopeSpeedModifier();
    virtual ~SlopeSpeedModifier();
    //@}

    //! @name Operations
    //@{
    bool ComputeLocalSlope( const PHY_RawVisionData& data, const MT_Vector2D& from, const MT_Vector2D& to );
    void ModifySpeed( double& rSpeed, double rSlopeDeceleration, double rMaxSlope, const MT_Vector2D& to ) const;
    const T_Slopes& GetSlopes() const;
    //@}

private:
    //! @name Member data
    //@{
    T_Slopes slopes_;
    double currentSlope_;
    //@}
};

#endif // __SlopeSpeedModifier_h_
