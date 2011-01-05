// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __BurnSurfaceCapacity_h_
#define __BurnSurfaceCapacity_h_

#include "ObjectCapacity_ABC.h"

namespace xml
{
    class xistream;
}

class MIL_BurningCells;

// =============================================================================
/** @class  BurnSurfaceCapacity
    @brief  Allows to burn a surface defined with a polygon.
			This polygon is roughly subdivised in fixed size cells.
			The real burn is done by sub objects with BurnCellsCapacity.
	@see "Feu sauvage" in 4311340-Mod�les de feu.doc
*/
// Created: BCI 2010-12-17
// =============================================================================

class BurnSurfaceCapacity : public ObjectCapacity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit BurnSurfaceCapacity( xml::xistream& xis );
    explicit BurnSurfaceCapacity();
    virtual ~BurnSurfaceCapacity();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    virtual void Register( MIL_Object_ABC& object );
    //@}

    //! @name Capacity creation
    //@{
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Finalize( MIL_Object_ABC& object );
    //@}

    //! @name Update
    //@{
    virtual void Update( MIL_Object_ABC& object, unsigned int time );
    //@}

private:
    //! @name Copy constructor
    //@{
    explicit BurnSurfaceCapacity( const BurnSurfaceCapacity& );
    //@}

    //! @name Initialize Data
    //@{
    void InitializeData( xml::xistream& xis );
    //@}

    //! @name Private member
    //@{
	MIL_BurningCells& burningCells_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( BurnSurfaceCapacity )

#endif // __BurnSurfaceCapacity_h_
