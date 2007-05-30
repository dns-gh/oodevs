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

namespace kernel
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
    explicit WorldParameters( const ExerciseConfig& config ); 
    virtual ~WorldParameters();
    //@}

    //! @name Operations
    //@{
    void Load( const ExerciseConfig& config );

    geometry::Point2f Clip( const geometry::Point2f& point ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadWorld( const std::string& world );
    //@}

public:
    //! @name Member data
    //@{
    float latitude_, longitude_;
    float width_, height_;
    std::string terrainDirectory_;
    std::string graphicsDirectory_;
    std::string detection_;
    std::string geoid_;
    //@}
};

}

#endif // __WorldParameters_h_
