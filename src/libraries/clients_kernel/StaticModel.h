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

namespace tools
{
    class ExerciseConfig;
}

namespace kernel
{
    class AgentTypes;
    class ObjectTypes;
    class CoordinateConverter_ABC;
    class CoordinateSystems;
    class FormationLevels;
    class AtlasNatures;

// =============================================================================
/** @class  StaticModel
    @brief  StaticModel
*/
// Created: SBO 2010-05-10
// =============================================================================
class StaticModel
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

private:
    //! @name Copy/Assignment
    //@{
    StaticModel( const StaticModel& );            //!< Copy constructor
    StaticModel& operator=( const StaticModel& ); //!< Assignment operator
    //@}

public:
    //! @name Member data
    //@{
    kernel::CoordinateSystems&       coordinateSystems_;
    kernel::CoordinateConverter_ABC& coordinateConverter_;
    kernel::AgentTypes&              types_;
    kernel::ObjectTypes&             objectTypes_;
    kernel::FormationLevels&         levels_;
    kernel::AtlasNatures&            atlasNatures_;
    //@}
};

}

#endif // __kernel_StaticModel_h_
