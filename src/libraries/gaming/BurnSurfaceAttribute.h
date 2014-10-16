// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BurnSurfaceAttribute_h_
#define __BurnSurfaceAttribute_h_

#include "clients_gui/Drawable_ABC.h"
#include "clients_gui/Pickable_ABC.h"
#include "clients_kernel/ObjectExtensions.h"
#include <tools/Resolver_ABC.h>
#include <geometry/types.h>

namespace kernel
{
    class Controller;
    class Entity_ABC;
}

namespace sword
{
    enum EnumBurningCellPhase;
}

class Publisher_ABC;

// =============================================================================
/** @class  BurnSurfaceAttribute
    @brief  BurnSurfaceAttribute
*/
// Created: BCI 2010-12-13
// =============================================================================
class BurnSurfaceAttribute : public kernel::BurnSurfaceAttribute_ABC
                           , public gui::Drawable_ABC
                           , public gui::Pickable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             BurnSurfaceAttribute( kernel::Controller& controller, Publisher_ABC& publisher, kernel::Entity_ABC& entity );
    virtual ~BurnSurfaceAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    virtual void TerrainPicked( const geometry::Point2f& terrainCoordinates, QStringList& outputInfos );
    virtual void Display( kernel::Displayer_ABC& ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef geometry::Point2< int > BurningCellOrigin;

    struct BurningCell
    {
        BurningCell()
            : ignitionEnergy_( -1 )
            , ignitionThreshold_( -1 )
            , combustionEnergy_( -1 )
            , currentHeat_( -1 )
            , maxCombustionEnergy_( -1 )
        {
        }
        sword::EnumBurningCellPhase phase_;
        int ignitionEnergy_;
        int ignitionThreshold_;
        int combustionEnergy_;
        int currentHeat_;
        int maxCombustionEnergy_;
    };
    //@}

private:
    //! @name Copy/Assignment
    //@{
    BurnSurfaceAttribute( const BurnSurfaceAttribute& );            //!< Copy constructor
    BurnSurfaceAttribute& operator=( const BurnSurfaceAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::ObjectKnowledgeUpdate& );
    virtual void DoUpdate( const sword::ObjectUpdate& );

    template< typename T >
    void UpdateData( const T& message );
    template< typename T >
    void UpdateBurningCellData( const T& message );

    BurningCellOrigin ComputeCellOriginFromPoint( const geometry::Point2f& terrainCoordinates ) const;
    //@}

private:
    //! @name Member data
    //@{
    typedef std::map< BurningCellOrigin, BurningCell > BurningCellsByCoordinatesMap;
    BurningCellsByCoordinatesMap burningCellsByCoordinates_;
    kernel::Controller& controller_;
    Publisher_ABC& publisher_;
    kernel::Entity_ABC& entity_;
    int cellSize_;
    //@}
};

#endif // __BurnSurfaceAttribute_h_
