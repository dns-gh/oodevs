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

#include "MIL.h"
#include "ObjectCapacity_ABC.h"
#include "ConstructionCapacity.h"

namespace xml
{
    class xistream;
}

class PHY_DotationCategory;
class PHY_ConsumptionType;

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
             ImprovableCapacity();
             ImprovableCapacity( const PHY_ConsumptionType& consumption, ConstructionCapacity::E_UnitType type, xml::xistream& xis );
    virtual ~ImprovableCapacity();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    virtual void Register( MIL_Object_ABC& object );
    virtual void Finalize( MIL_Object_ABC& object );
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    void Mine( MIL_Object_ABC& object );
    //@}

    //! @name Accessors
    //@{
    const PHY_ConsumptionType&  GetDefaultConsumptionMode() const;
    const PHY_DotationCategory* GetDotationCategory() const;
    unsigned int                        GetMaxDotation() const;
    ConstructionCapacity::E_UnitType GetUnit() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ImprovableCapacity( const ImprovableCapacity& );            //!< Copy constructor
    ImprovableCapacity& operator=( const ImprovableCapacity& ); //!< Assignment operator
    //@}

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
    //@}
};

BOOST_CLASS_EXPORT_KEY( ImprovableCapacity )

#endif // __ImprovableCapacity_h_
