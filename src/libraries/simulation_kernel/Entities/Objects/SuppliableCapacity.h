// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __SuppliableCapacity_h_
#define __SuppliableCapacity_h_

#include "MIL.h"
#include "ObjectCapacity_ABC.h"
#include "ConstructionCapacity.h"
#include <boost/serialization/export.hpp>

namespace xml
{
    class xistream;
}

class PHY_DotationCategory;
class PHY_ConsumptionType;
class TER_Localisation;

// =============================================================================
/** @class  SuppliableCapacity
    @brief  SuppliableCapacity
*/
// Created: LDC 2014-05-06
// =============================================================================
class SuppliableCapacity : public ObjectCapacity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SuppliableCapacity();
             SuppliableCapacity( const PHY_ConsumptionType& consumption, ConstructionCapacity::E_UnitType type, xml::xistream& xis );
    virtual ~SuppliableCapacity();
    //@}

     //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    unsigned int GetDotationNumber( const TER_Localisation& location ) const;
    template <typename T> void FinalizeAttribute( MIL_Object_ABC& object );
    void SetDensity( double density, const TER_Localisation& location );
    //@}
    
    //! @name Accessors
    //@{
    const PHY_ConsumptionType&  GetDefaultConsumptionMode() const;
    const PHY_DotationCategory* GetDotationCategory() const;
    unsigned int GetMaxDotation() const;
    ConstructionCapacity::E_UnitType GetUnit() const;
    double GetDensity( const TER_Localisation& location ) const;
    //@}

protected:
    //! @name Copy constructor
    //@{
    SuppliableCapacity( const SuppliableCapacity& );
    //@}

private:
    //! @name Helpers
    //@{
    void ReadDotation( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    const PHY_ConsumptionType* default_;
    ConstructionCapacity::E_UnitType unitType_;
    const PHY_DotationCategory* dotation_;
    unsigned int nFullNbrDotation_;
    bool finalised_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( SuppliableCapacity )

// -----------------------------------------------------------------------------
// Name: SuppliableCapacity::Finalize
// Created: LGY 2012-01-25
// -----------------------------------------------------------------------------
template <typename T>
void SuppliableCapacity::FinalizeAttribute( MIL_Object_ABC& object )
{
    if( dotation_ )
    {
        TER_Localisation localisation = object.GetLocalisation();
        T& attribute = object.GetAttribute< T >();
        attribute.SetDensity( GetDensity( localisation ) );
        nFullNbrDotation_ = GetDotationNumber( localisation );
        attribute.SetMaxDotations( *dotation_, nFullNbrDotation_ );
    }
    finalised_ = true; // $$$$ LDC FIXME the nFullNbrDotation_ should be in the object or attribute not the capacity
}

#endif // __SuppliableCapacity_h_
