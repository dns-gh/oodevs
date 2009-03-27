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

class TerrainData;

// $$$$ TODO : push into pathfind rules

class TerrainHeuristicCapacity : public ObjectCapacity_ABC
{
public:
	//! @name Constructors/Destructor
    //@{
	explicit TerrainHeuristicCapacity( xml::xistream& xis );
	         TerrainHeuristicCapacity();
	virtual ~TerrainHeuristicCapacity();
	//@}

     //! @name CheckPoints
    //@{    
    template< typename Archive > void serialize( Archive&, const uint );
    virtual void Register( Object& /*object*/ );
    //@}

	//! @name virtual
    //@{
    virtual void Instanciate( Object& object ) const;
    //@}

    //! @name 
    //@{
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
    //! @name Data members
    //@{
    T_EnvironmentScoreMap       environmentScores_;
    //@}
};

#endif // __TerrainHeuristicCapacity_h_