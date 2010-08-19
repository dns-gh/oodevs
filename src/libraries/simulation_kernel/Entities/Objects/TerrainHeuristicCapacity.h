// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TerrainHeuristicCapacity_h_
#define __TerrainHeuristicCapacity_h_

#include "ObjectCapacity_ABC.h"

namespace xml
{
    class xistream;
}

class TerrainData;

// =============================================================================
/** @class  TerrainHeuristicCapacity
    @brief  TerrainHeuristicCapacity
*/
// Created: MGD 2009-03-05
// =============================================================================
class TerrainHeuristicCapacity : public ObjectCapacity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             TerrainHeuristicCapacity();
    explicit TerrainHeuristicCapacity( xml::xistream& xis );
    virtual ~TerrainHeuristicCapacity();
    //@}

     //! @name Operations
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Register( MIL_Object_ABC& object );
    int ComputePlacementScore( const MT_Vector2D& pos, const TerrainData& nPassability ) const;
    //@}

private:
    //! @name Copy
    //@{
    TerrainHeuristicCapacity( const TerrainHeuristicCapacity& );
    //@}

    //! @name Helpers
    //@{
    void ReadTerrain( xml::xistream& xis );
    //@}

private:
    //! @name
    //@{
    typedef std::pair < TerrainData, int >          T_ScorePair;
    typedef std::vector< T_ScorePair >              T_EnvironmentScoreMap;
    typedef T_EnvironmentScoreMap::const_iterator CIT_EnvironmentScoreMap;
    //@}

private:
    //! @name Member data
    //@{
    T_EnvironmentScoreMap environmentScores_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( TerrainHeuristicCapacity )

#endif // __TerrainHeuristicCapacity_h_
