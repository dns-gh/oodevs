// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef aar_StaticModel_h
#define aar_StaticModel_h

#include "StaticModel_ABC.h"
#include <memory>
namespace extractors
{
    struct PowerExtractor_ABC;
}

namespace tools
{
    class ExerciseConfig;
}

namespace aar
{
    class PowerIndicators;
    class PowerModificator;

// =============================================================================
/** @class  StaticModel
    @brief  Static model for Action After Review
*/
// Created: ABR 2011-02-09
// =============================================================================
class StaticModel : public StaticModel_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit StaticModel( const tools::ExerciseConfig& config );
    virtual ~StaticModel();
    //@}

    //! @name Operations
    //@{
    virtual float ComputePower( const std::map< int, sword::EquipmentDotations_EquipmentDotation >& message, const extractors::PowerExtractor_ABC& Extractor ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< aar::PowerIndicators > powerIndicators_;
    std::unique_ptr< aar::PowerModificator > powerModificators_;
    //@}
};

} // namespace aar

#endif // aar_StaticModel_h
