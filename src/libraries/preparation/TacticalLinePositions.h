// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __TacticalLinePositions_h_
#define __TacticalLinePositions_h_

#include "clients_kernel/TacticalLinePositions_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_gui/ShapeHandler_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace xml
{
    class xistream;
}
namespace gui
{
    class GlTools_ABC;
    class ParametersLayer;
    class Viewport_ABC;
}

// =============================================================================
/** @class  TacticalLinePositions
    @brief  TacticalLine Positions
*/
// Created: SBO 2006-11-06
// =============================================================================
class TacticalLinePositions : public kernel::TacticalLinePositions_ABC
                            , public kernel::Serializable_ABC
                            , public gui::ShapeHandler_ABC
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
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    void Edit( gui::ParametersLayer& parameters );
    void Translate( const geometry::Point2f& from, const geometry::Vector2f& translation, float precision );
    void InsertPoint( const geometry::Point2f& point, float precision );
    void RemovePoint( const geometry::Point2f& point, float precision );
    //@}

    //! @name Serializable_ABC
    //@{
    virtual void SerializeAttributes( xml::xostream& ) const;
    //@}

    //! @name ShapeHandler_ABC
    //@{
    virtual void Handle( kernel::Location_ABC& location );
    virtual void Reset();
    //@}

private:
    //! @name Helpers
    //@{
    void ReadPoint( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    bool isEditing_;
    //@}
};

#endif // __TacticalLinePositions_h_
