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

#include <tools/Path.h>

namespace xml
{
    class xistream;
}

namespace tools
{
    class Loader_ABC;

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
             WorldParameters( const Loader_ABC& fileLoader, const Path& dataset, const Path& physical, const Path& terrainFile, const Path& populationFile );
    virtual ~WorldParameters();
    //@}

    // Returns the smallest UTM zone number covered by the world.
    std::string GetUtmZone() const;

private:
    //! @name Helpers
    //@{
    void ReadTerrain( const Path& terrainFile, const Path& dataset,
            const Path& physical, xml::xistream& xis );
    void ReadPopulation( const Path& populationFile, xml::xistream& xis );
    void ReadWorld( const Path& world );
    //@}

public:
    //! @name Member data
    //@{
    float latitude_, longitude_;
    float width_, height_;
    Path graphicsDirectory_;
    Path detectionDirectory_;
    Path pathfindGraph_;
    Path pathfindLinks_;
    Path pathfindNodes_;
    Path detection_;
    Path populationGraph_;
    bool terrainSamePhysical_;
    //@}

private:
    std::string minUtmZone_;
};

}

#endif // __WorldParameters_h_
