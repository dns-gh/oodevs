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

namespace xml
{
    class xistream;
}

class Deserializer;
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
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

	//! @name Operations
    //@{
    virtual void Instanciate( Object& object ) const;	
    virtual void Register( Object& object );

    void Construct( Object& object );
    void Construct( Object& object, float rDeltaPercentage );
    void Destroy( Object& object );
    void Destroy( Object& object, float rDeltaPercentage );
    //@}

    //! @name Accessors
    //@{
    const PHY_ConsumptionType&  GetDefaultConsumptionMode() const;
    const PHY_DotationCategory* GetDotationCategory() const;
    uint                        GetMaxDotation() const;
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
    void ChangeConstructionPercentage( Object& object, float rNewConstructionPercentage );
    //@}

private:
    //! @name Member data
    //@{
    const PHY_ConsumptionType* default_;
    const PHY_DotationCategory* dotation_;
    uint nFullNbrDotation_;
    ConstructionCapacity::E_UnitType unitType_;
    //@}
};

#endif // __BuildableCapacity_h_