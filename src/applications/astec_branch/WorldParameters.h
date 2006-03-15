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
    explicit WorldParameters( const std::string& scipioXml );
    virtual ~WorldParameters();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    WorldParameters( const WorldParameters& );            //!< Copy constructor
    WorldParameters& operator=( const WorldParameters& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void ReadTerrain( const std::string& terrain );
    void ReadWorld( const std::string& world );
    //@}

protected:
    //! @name Member data
    //@{
    float latitude_, longitude_;
    float width_, height_;
    std::string dataDirectory_;
    std::string terrainDirectory_;
    std::string graphicsDirectory_;
    std::string detection_;
    std::string geoid_;
    //@}
};

#endif // __WorldParameters_h_
