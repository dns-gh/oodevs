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

#include "clients_kernel/DetectionMapIterator.h"

namespace gui
{
// =============================================================================
/** @class  VisionLine
    @brief  VisionLine
*/
// Created: AGE 2006-04-14
// =============================================================================
class VisionLine : boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             VisionLine( const kernel::DetectionMap& map, const geometry::Point2f& from, const geometry::Point2f& to, float height );
    virtual ~VisionLine();
    //@}

    //! @name Operations
    //@{
    void Increment();
    bool IsDone() const;

    bool IsInGround() const;
    bool IsInTown() const;
    bool IsInForest() const;

    short Elevation() const;
    float Length() const;

    geometry::Point2f CurrentPoint() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::DetectionMapIterator iterator_;
    float fromAltitude_;
    float toAltitude_;
    float totalDistance_;
    float advancedDistance_;
    float currentAltitude_;
    float length_;
    //@}
};

}

#endif // __VisionLine_h_
