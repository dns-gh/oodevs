// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TacticalLinePositions_h_
#define __TacticalLinePositions_h_

#include "clients_gui/TacticalLinePositions_ABC.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  TacticalLinePositions
    @brief  TacticalLine Positions
*/
// Created: SBO 2006-11-06
// =============================================================================
class TacticalLinePositions : public gui::TacticalLinePositions_ABC
                            , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             TacticalLinePositions( const T_PointVector& pointList, const kernel::CoordinateConverter_ABC& converter, const kernel::TacticalLine_ABC& owner );
             TacticalLinePositions( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter, const kernel::TacticalLine_ABC& owner );
    virtual ~TacticalLinePositions();
    //@}

    //! @name Operations
    //@{
    virtual void SerializeAttributes( xml::xostream& ) const;
    void Translate( const geometry::Point2f& from, const geometry::Vector2f& translation, float precision );
    void InsertPoint( const geometry::Point2f& point, float precision );
    void RemovePoint( const geometry::Point2f& point, float precision );
    //@}

private:
    //! @name Helpers
    //@{
    void ReadPoint( xml::xistream& xis );
    //@}
};

#endif // __TacticalLinePositions_h_
