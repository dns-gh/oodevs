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

class Deserializer;
class PHY_DotationCategory;
class PHY_ConsumptionType;

class BuildableCapacity : public ObjectCapacity_ABC
{
public:
	//! @name Constructors/Destructor
    //@{    
    explicit BuildableCapacity( const PHY_ConsumptionType& consumption, ConstructionCapacity::E_UnitType type, xml::xistream& xis );	
             BuildableCapacity();
    virtual ~BuildableCapacity();
	//@}

     //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    virtual void Register( Object& /*object*/ );
    //@}

	//! @name virtual
    //@{
    virtual void Instanciate( Object& object ) const;	
    //@}

    //! @name Accessor
    //@{
    const PHY_ConsumptionType&  GetDefaultConsumptionMode() const;
    const PHY_DotationCategory* GetDotationCategory() const;
    uint                        GetMaxDotation() const;
    ConstructionCapacity::E_UnitType GetUnit() const;
    //@}

    //! @name 
    //@{    
    void Construct( Object& object, float rDeltaPercentage );
    void Destroy( Object& object, float rDeltaPercentage );    
    //@}

    //! @name 
    //@{
    void Construct( Object& object );
    void Destroy( Object& object );
    //@}

private:
    //! @name Copy
    //@{
    BuildableCapacity( const BuildableCapacity& );
    //@}

    //! @name 
    //@{
    void ChangeConstructionPercentage( Object& object, float rNewConstructionPercentage );
    //@}

    //! @name 
    //@{
    void ReadDotation( xml::xistream& xis );
    //@}

private:
    //! @name Data members
    //@{
    uint nFullNbrDotation_;
    const PHY_DotationCategory* dotation_;
    const PHY_ConsumptionType* default_;
    ConstructionCapacity::E_UnitType unitType_;
    //@}
};

#endif // __BuildableCapacity_h_