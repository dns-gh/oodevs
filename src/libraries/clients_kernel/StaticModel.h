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
    class Controllers;
    class CoordinateConverter_ABC;
    class CoordinateSystems;
    class DetectionMap;
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
    explicit StaticModel();
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
    CoordinateSystems&       coordinateSystems_;
    CoordinateConverter_ABC& coordinateConverter_;
    AgentTypes&              types_;
    ObjectTypes&             objectTypes_;
    FormationLevels&         levels_;
    ExtensionTypes&          extensions_;
    AtlasNatures&            atlasNatures_;
    AccommodationTypes&      accommodationTypes_;
    DetectionMap&            detection_;
    //@}
};

}

#endif // __kernel_StaticModel_h_
