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

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Pickable_ABC.h"
#include "tools/Resolver_ABC.h"
#include <geometry/types.h>

namespace kernel
{
    class Controller;
}

namespace sword
{
	enum EnumBurningCellPhase;
}

// =============================================================================
/** @class  BurnSurfaceAttribute
    @brief  BurnSurfaceAttribute
*/
// Created: BCI 2010-12-13
// =============================================================================
class BurnSurfaceAttribute : public kernel::BurnSurfaceAttribute_ABC
						   , public kernel::Drawable_ABC
						   , public kernel::Pickable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             BurnSurfaceAttribute( kernel::Controller& controller );
    virtual ~BurnSurfaceAttribute();
    //@}

    //! @name Overrides of kernel::Drawable_ABC
    //@{
	virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

    //! @name Overrides of kernel::Pickable_ABC
    //@{
	virtual void TerrainPicked( const geometry::Point2f& terrainCoordinates, QStringList& outputInfos );
    //@}

    //! @name Overrides of kernel::Displayable_ABC
    //@{
    virtual void Display         ( kernel::Displayer_ABC& ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& ) const;
    //@}
private:
    //! @name Copy/Assignment
    //@{
    BurnSurfaceAttribute( const BurnSurfaceAttribute& );            //!< Copy constructor
    BurnSurfaceAttribute& operator=( const BurnSurfaceAttribute& ); //!< Assignment operator
    //@}

	typedef geometry::Point2< int > BurningCellOrigin;

	struct BurningCell
	{
		sword::EnumBurningCellPhase phase_;
		int ignitionEnergy_;
		int ignitionThreshold_;
		int combustionEnergy_;
		int currentHeat_;
		int maxCombustionEnergy_;
	};

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

    //! @name Member data
    //@{
	typedef std::map< BurningCellOrigin, BurningCell > BurningCellsByCoordinatesMap;
	BurningCellsByCoordinatesMap burningCellsByCoordinates_;
    kernel::Controller& controller_;
	int cellSize_;
    //@}
};

#endif // __BurnSurfaceAttribute_h_
