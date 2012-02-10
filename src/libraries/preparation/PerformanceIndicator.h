// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __PerformanceIndicator_h_
#define __PerformanceIndicator_h_

#include <boost/noncopyable.hpp>

namespace tools
{
    class ExerciseConfig;
}

namespace xml
{
    class xistream;
}

class Model;

// =============================================================================
/** @class  PerformanceIndicator
    @brief  PerformanceIndicator
*/
// Created: MMC 2012-02-02
// =============================================================================
class PerformanceIndicator : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             PerformanceIndicator( const Model& model );
    virtual ~PerformanceIndicator();
    //@}

    //! @name Types
    //@{
    struct IndicatorValues 
    {
        IndicatorValues(): limit_( 0 ), units_( 0 ), blocs_( 0 ), objects_( 0 ), terrainLoad_( 0 ), knowledges_( 0 ) {}

        unsigned int limit_;
        unsigned int units_;
        unsigned int blocs_;
        unsigned int objects_;
        unsigned int terrainLoad_;
        unsigned int knowledges_;
    };
    //@}

    //! @name Operations
    //@{
    void Load( const tools::ExerciseConfig& config, const std::string& file );
    float ComputeLoadIndicator();
    float ComputeLoadIndicator( PerformanceIndicator::IndicatorValues& values );
    bool IsLoaded();
    //@}

private:

    //! @name Helpers
    //@{
    void Read( xml::xistream& xis );
    //@}

    const Model& model_;
    float limit_;
    float globalFactor_;
    float unit_;
    float urban_;
    float object_;
    float terrain_;
    float knowledge_;
    float unitknowledge_;
    bool  loaded_;
    float terrainMemorySizeIndicator_;
};

#endif // __PerformanceIndicator_h_
