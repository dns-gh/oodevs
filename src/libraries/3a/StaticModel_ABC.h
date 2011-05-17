// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef aar_StaticModel_ABC_h
#define aar_StaticModel_ABC_h

#include <boost/noncopyable.hpp>
#include <map>

namespace extractors
{
    struct PowerExtractor_ABC;
}

namespace sword
{
    class UnitAttributes;
    class EquipmentDotations_EquipmentDotation;
}

namespace aar
{

// =============================================================================
/** @class  StaticModel_ABC
    @brief  Abstract static model for Action After Review
*/
// Created: ABR 2011-02-11
// =============================================================================
class StaticModel_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             StaticModel_ABC() {}
    virtual ~StaticModel_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual float ComputePower( const std::map< int, sword::EquipmentDotations_EquipmentDotation >& message, const extractors::PowerExtractor_ABC& extractor ) const = 0;
    //@}
};

} // namespace aar

#endif // aar_StaticModel_ABC_h
