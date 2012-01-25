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
#include "ObjectCapacity_ABC.h"
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

// =============================================================================
/** @class  BuildableCapacity
    @brief  BuildableCapacity
*/
// Created: JCR 2008-05-30
// =============================================================================
class BuildableCapacity : public ObjectCapacity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             BuildableCapacity();
    explicit BuildableCapacity( const PHY_ConsumptionType& consumption, ConstructionCapacity::E_UnitType type, xml::xistream& xis );
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
    virtual void Finalize( MIL_Object_ABC& object );
    virtual void Register( MIL_Object_ABC& object );

    void Construct( MIL_Object_ABC& object );
    void Construct( MIL_Object_ABC& object, float rDeltaPercentage );
    void Destroy( MIL_Object_ABC& object );
    void Destroy( MIL_Object_ABC& object, float rDeltaPercentage );
    //@}

    //! @name Accessors
    //@{
    const PHY_ConsumptionType&  GetDefaultConsumptionMode() const;
    const PHY_DotationCategory* GetDotationCategory() const;
    unsigned int GetMaxDotation() const;
    ConstructionCapacity::E_UnitType GetUnit() const;
    //@}

private:
    //! @name Copy
    //@{
    BuildableCapacity( const BuildableCapacity& );
    //@}

    //! @name Helpers
    //@{
    void ReadDotation( xml::xistream& xis );
    void ChangeConstructionPercentage( MIL_Object_ABC& object, float rNewConstructionPercentage );
    //@}

private:
    //! @name Member data
    //@{
    const PHY_ConsumptionType* default_;
    const PHY_DotationCategory* dotation_;
    unsigned int nFullNbrDotation_;
    ConstructionCapacity::E_UnitType unitType_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( BuildableCapacity )

#endif // __BuildableCapacity_h_
