// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __WorldParameters_h_
#define __WorldParameters_h_

#include "Path.h"

namespace xml
{
    class xistream;
}

namespace tools
{
    class SchemaWriter_ABC;
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

    //! @name Operations
    //@{
    void Serialize( const Path& filename, const tools::SchemaWriter_ABC& schemaWriter ) const;
    void Purge();
    //@}

private:
    //! @name Helpers
    //@{
    void ReadTerrain( const Path& terrainFile, xml::xistream& xis );
    void ReadPopulation( const Path& populationFile, xml::xistream& xis );
    void ReadWorld( const Path& world );
    void ReadExtent( const Path& extent );
    void InitExtent();
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
    Path urban_;
    Path populationGraph_;
    Path dataset_;
    Path physical_;
    std::string xMin_;
    std::string xMax_;
    std::string yMin_;
    std::string yMax_;
    std::vector< unsigned char > utmZones_;
    bool terrainSamePhysical_;
    //@}
};

}

#endif // __WorldParameters_h_
