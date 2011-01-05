// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BurnCellsCapacity_h_
#define __BurnCellsCapacity_h_

#include "ObjectCapacity_ABC.h"
#include "MIL_InteractiveContainer_ABC.h"

namespace xml
{
    class xistream;
}

class MIL_PopulationElement_ABC;
class MIL_Agent_ABC;

// =============================================================================
/** @class  BurnCellsCapacity
	@see BurnSurfaceCapacity
	@see "Feu sauvage" in 4311340-Modèles de feu.doc
*/
// Created: BCI 2010-12-17
// =============================================================================

class BurnCellsCapacity : public ObjectCapacity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit BurnCellsCapacity( xml::xistream& xis );
             BurnCellsCapacity();
    virtual ~BurnCellsCapacity();
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
    explicit BurnCellsCapacity( const BurnCellsCapacity& );
    //@}

    //! @name Initialize Data
    //@{
    void InitializeData( xml::xistream& xis );
    //@}

    //! @name Private member
    //@{
    //@}
};

BOOST_CLASS_EXPORT_KEY( BurnCellsCapacity )

#endif // __BurnCellsCapacity_h_
