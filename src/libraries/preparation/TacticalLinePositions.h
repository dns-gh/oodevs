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
    class GLView_ABC;
    class ParametersLayer;
    class Viewport_ABC;
}

// =============================================================================
/** @class  TacticalLinePositions
    @brief  TacticalLine Positions
*/
// Created: SBO 2006-11-06
// =============================================================================
class TacticalLinePositions : public gui::TacticalLinePositions_ABC
                            , public kernel::Serializable_ABC
                            , public gui::ShapeHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             TacticalLinePositions( kernel::Controller& controller, const T_PointVector& pointList, const kernel::CoordinateConverter_ABC& converter,
                                    const kernel::TacticalLine_ABC& owner );
             TacticalLinePositions( kernel::Controller& controller, xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter,
                                    const kernel::TacticalLine_ABC& owner );
    virtual ~TacticalLinePositions();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
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
