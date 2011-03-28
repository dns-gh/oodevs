// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __kernel_StaticModel_h_
#define __kernel_StaticModel_h_

#include <boost/noncopyable.hpp>

namespace tools
{
    class ExerciseConfig;
}

namespace kernel
{
    class AccommodationTypes;
    class AgentTypes;
    class AtlasNatures;
    class CoordinateConverter_ABC;
    class CoordinateSystems;
    class ExtensionTypes;
    class FormationLevels;
    class ObjectTypes;

// =============================================================================
/** @class  StaticModel
    @brief  Kernel Static Model
*/
// Created: SBO 2010-05-10
// =============================================================================
class StaticModel : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             StaticModel();
    virtual ~StaticModel();
    //@}

    //! @name Operations
    //@{
    virtual void Load( const tools::ExerciseConfig& config );
    virtual void Purge();
    //@}

public:
    //! @name Member data
    //@{
    kernel::CoordinateSystems&       coordinateSystems_;
    kernel::CoordinateConverter_ABC& coordinateConverter_;
    kernel::AgentTypes&              types_;
    kernel::ObjectTypes&             objectTypes_;
    kernel::FormationLevels&         levels_;
    kernel::ExtensionTypes&          extensionTypes_;
    kernel::AtlasNatures&            atlasNatures_;
    kernel::AccommodationTypes&      accomodationTypes_;
    //@}
};

}

#endif // __kernel_StaticModel_h_
