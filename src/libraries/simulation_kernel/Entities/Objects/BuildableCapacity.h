// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BuildableCapacity_h_
#define __BuildableCapacity_h_

#include "simulation_kernel/MIL.h"
#include "SuppliableCapacity.h"
#include "ConstructionCapacity.h"
#include <boost/serialization/export.hpp>

namespace xml
{
    class xistream;
}

class Deserializer;
class MIL_Object_ABC;
class PHY_DotationCategory;
class PHY_ConsumptionType;
class TER_Localisation;

// =============================================================================
/** @class  BuildableCapacity
    @brief  BuildableCapacity
*/
// Created: JCR 2008-05-30
// =============================================================================
class BuildableCapacity : public SuppliableCapacity
{
public:
    //! @name Constructors/Destructor
    //@{
             BuildableCapacity();
             BuildableCapacity( const PHY_ConsumptionType& consumption, ConstructionCapacity::E_UnitType type, xml::xistream& xis );
    virtual ~BuildableCapacity();
    //@}

     //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Register( MIL_Object_ABC& object );

    void Construct( MIL_Object_ABC& object );
    void Construct( MIL_Object_ABC& object, float rDeltaPercentage );
    void Destroy( MIL_Object_ABC& object );
    //@}

private:
    //! @name Copy
    //@{
    BuildableCapacity( const BuildableCapacity& );
    //@}

    //! @name Helpers
    //@{
    void ChangeConstructionPercentage( MIL_Object_ABC& object, float rNewConstructionPercentage );
    //@}
};

BOOST_CLASS_EXPORT_KEY( BuildableCapacity )

#endif // __BuildableCapacity_h_
