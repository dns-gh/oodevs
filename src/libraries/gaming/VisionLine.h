// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __VisionLine_h_
#define __VisionLine_h_

#include "astec_kernel/DetectionMapIterator.h"

// =============================================================================
/** @class  VisionLine
    @brief  VisionLine
*/
// Created: AGE 2006-04-14
// =============================================================================
class VisionLine : public DetectionMapIterator
{

public:
    //! @name Constructors/Destructor
    //@{
             VisionLine( const DetectionMap& map, const geometry::Point2f& from, const geometry::Point2f& to, float height );
    virtual ~VisionLine();
    //@}

    //! @name Operations
    //@{
    void Increment();

    bool IsInGround() const;
    bool IsInTown() const;
    bool IsInForest() const;

    float Length() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    VisionLine( const VisionLine& );            //!< Copy constructor
    VisionLine& operator=( const VisionLine& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    float fromAltitude_;
    float toAltitude_;
    float totalDistance_;
    float advancedDistance_;
    float currentAltitude_;
    float length_;
    //@}
};

#endif // __VisionLine_h_
