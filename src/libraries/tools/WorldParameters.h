// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __WorldParameters_h_
#define __WorldParameters_h_

namespace xml
{
    class xistream;
}

namespace tools
{
    class ExerciseConfig;

// =============================================================================
/** @class  WorldParameters
    @brief  WorldParameters
*/
// Created: AGE 2006-03-15
// =============================================================================
class WorldParameters
{
public:
    //! @name Constructors/Destructor
    //@{
             WorldParameters();
    explicit WorldParameters( const tools::ExerciseConfig& config );
    virtual ~WorldParameters();
    //@}

    //! @name Operations
    //@{
    void Load( const tools::ExerciseConfig& config );

    geometry::Point2f Clip( const geometry::Point2f& point ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadTerrain( const tools::ExerciseConfig& config, xml::xistream& xis );
    void ReadPopulation( const tools::ExerciseConfig& config, xml::xistream& xis );
    void ReadWorld( const std::string& world );
    void ReadExtent( const std::string& extent );
    void InitExtent();
    //@}

public:
    //! @name Member data
    //@{
    float latitude_, longitude_;
    float width_, height_;
    std::string graphicsDirectory_;
    std::string detectionDirectory_;
    std::string pathfindGraph_;
    std::string pathfindLinks_;
    std::string pathfindNodes_;
    std::string detection_;
    std::string urban_;
    std::string populationGraph_;
    std::string xMin_;
    std::string xMax_;
    std::string yMin_;
    std::string yMax_;
    std::vector< unsigned char > utmZones_;
    //@}
};

}

#endif // __WorldParameters_h_
