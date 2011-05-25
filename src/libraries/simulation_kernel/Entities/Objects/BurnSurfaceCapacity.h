// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BurnSurfaceCapacity_h_
#define __BurnSurfaceCapacity_h_

#include "ObjectCapacity_ABC.h"
#include "MIL_InteractiveContainer_ABC.h"

namespace xml
{
    class xistream;
}

class MIL_BurningCells;

// =============================================================================
/** @class  BurnSurfaceCapacity
    @brief  Allows to burn a surface defined with a polygon.
            This polygon is roughly subdivised in fixed size cells.
    @see "Feu sauvage" in 4311340-Modèles de feu.doc
*/
// Created: BCI 2010-12-17
// =============================================================================

class BurnSurfaceCapacity : public ObjectCapacity_ABC
                          , public MIL_InteractiveContainer_ABC
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

    //! @name Overrides of MIL_InteractiveContainer_ABC
    //@{
    //Agent
    virtual void ProcessAgentMovingInside( MIL_Object_ABC& object, MIL_Agent_ABC& agent );
    virtual void ProcessAgentInside( MIL_Object_ABC& object, MIL_Agent_ABC& agent );
    //Population
    virtual void ProcessPopulationInside( MIL_Object_ABC& object, MIL_PopulationElement_ABC& agent );
    //@}

private:
    //! @name Copy constructor
    //@{
    explicit BurnSurfaceCapacity( const BurnSurfaceCapacity& );
    //@}

    //! @name Private member
    //@{
    MIL_BurningCells& burningCells_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( BurnSurfaceCapacity )

#endif // __BurnSurfaceCapacity_h_
