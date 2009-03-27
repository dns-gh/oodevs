// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __ImprovableCapacity_h_
#define __ImprovableCapacity_h_

#include "simulation_kernel/MIL.h"
#include "ObjectCapacity_ABC.h"
#include "ConstructionCapacity.h"

namespace xml
{
    class xistream;
}

class PHY_DotationCategory;
class PHY_ConsumptionType;
class Deserializer;

// =============================================================================
/** @class  ImprovableCapacity
    @brief  ImprovableCapacity
*/
// Created: JCR 2008-05-30
// =============================================================================
class ImprovableCapacity : public ObjectCapacity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{    
    explicit ImprovableCapacity( const PHY_ConsumptionType& consumption, ConstructionCapacity::E_UnitType type, xml::xistream& xis );
             ImprovableCapacity();
    virtual ~ImprovableCapacity();
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
    void Mine( Object& object );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ImprovableCapacity( const ImprovableCapacity& );            //!< Copy constructor
    ImprovableCapacity& operator=( const ImprovableCapacity& ); //!< Assignment operator
    //@}

    //! @name 
    //@{
    void ReadDotation( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{    
    const PHY_ConsumptionType* default_;
    ConstructionCapacity::E_UnitType unitType_;    
    const PHY_DotationCategory* dotation_;
    uint nFullNbrDotation_;
    //@}
};

#endif // __ImprovableCapacity_h_
