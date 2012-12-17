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
             WorldParameters( const Loader_ABC& fileLoader, const std::string& dataset,
                              const std::string& physical, const std::string& terrainFile,
                              const std::string& populationFile );
    virtual ~WorldParameters();
    //@}

    //! @name Operations
    //@{
    void Serialize( const std::string& filename, const tools::SchemaWriter_ABC& schemaWriter ) const;
    void Purge();
    //@}

private:
    //! @name Helpers
    //@{
    void ReadTerrain( const std::string& terrainFile, xml::xistream& xis );
    void ReadPopulation( const std::string& populationFile, xml::xistream& xis );
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
    std::string dataset_;
    std::string physical_;
    std::vector< unsigned char > utmZones_;
    bool terrainSamePhysical_;
    //@}
};

}

#endif // __WorldParameters_h_
