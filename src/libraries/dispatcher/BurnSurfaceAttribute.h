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

#include "ObjectAttribute_ABC.h"
#include <geometry/types.h>

namespace sword
{
	enum EnumBurningCellPhase;
};

namespace dispatcher
{

// =============================================================================
/** @class  BurnSurfaceAttribute
    @brief  BurnSurfaceAttribute
*/
// Created: BCI 2010-12-22
// =============================================================================
class BurnSurfaceAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit BurnSurfaceAttribute( const sword::ObjectAttributes& asnMsg );
    virtual ~BurnSurfaceAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const sword::ObjectAttributes& asnMsg );
    virtual void Send  ( sword::ObjectAttributes& asnMsg ) const;
    //@}

private:

	//! @name Operations
    //@{
    void DoUpdate( const sword::ObjectAttributes& asnMsg );
    //@}


	typedef geometry::Point2< int > BurningCellOrigin;

	struct BurningCell
	{
		sword::EnumBurningCellPhase phase_;
		int ignitionEnergy_;
		int ignitionThreshold_;
		int combustionEnergyCount_;
		int combustionEnergySum_;
		int currentHeat_;
		int maxCombustionEnergy_;
	};

    //! @name Data members
    //@{
	typedef std::map< BurningCellOrigin, BurningCell > BurningCellsByCoordinatesMap;
	BurningCellsByCoordinatesMap burningCellsByCoordinates_;
	int cellSize_;
    //@}
};

}

#endif // __BurnSurfaceAttribute_h_
