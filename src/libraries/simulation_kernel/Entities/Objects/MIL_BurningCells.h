// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_BurningCells_h_
#define __MIL_BurningCells_h_

#include <geometry/types.h>
#include <map>

class MIL_Object_ABC;
class MIL_Agent_ABC;
class MIL_PopulationElement_ABC;
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;
class MT_Vector2D;
class MIL_EntityManager;
class TerrainData;

namespace sword
{
    enum EnumBurningCellPhase;
    class ObjectAttributes;
}

class MIL_BurningCell;
typedef geometry::Point2< int > MIL_BurningCellOrigin;

// =============================================================================
/** @class  MIL_BurningCells
    @see "Feu sauvage" in 4311340-Modèles de feu.doc
*/
// Created: BCI 2010-12-20
// =============================================================================
class MIL_BurningCells : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_BurningCells();
    virtual ~MIL_BurningCells();
    //@}

    //! @name Operations
    //@{
    void StartBurn( MIL_Object_ABC& object );
    void StartModifyBurn( MIL_Object_ABC& object );
    void StopModifyBurn( MIL_Object_ABC& object );
    void Update( MIL_Object_ABC& object, unsigned int time );
    double ComputePathCost( const MT_Vector2D& from, const MT_Vector2D& to ) const;
    bool IsTrafficable( const MT_Vector2D& from, const MT_Vector2D& to ) const;
    void OnRequest( double x, double y );
    //@}

    //! @name CheckPoints
    //@{
    void load( MIL_CheckPointInArchive& );
    void save( MIL_CheckPointOutArchive& ) const;
    void finalizeLoad( MIL_EntityManager& entityManager );
    void load( MIL_CheckPointInArchive&, unsigned int objectId, const unsigned int );
    void save( MIL_CheckPointOutArchive&, unsigned int objectId, const unsigned int ) const;
    //@}

    //! @name Dispatch & save
    //@{
    void SendFullState( sword::ObjectAttributes& asn, MIL_Object_ABC& object ) const;
    void SendUpdate( sword::ObjectAttributes& asn, MIL_Object_ABC& object ) const;
    void WriteODB( xml::xostream& xos, MIL_Object_ABC& ) const;
    //@}

    //! @name Attrition
    //@{
    void BurnAgent( MIL_Object_ABC& object, MIL_Agent_ABC& agent );
    void BurnPopulation( MIL_PopulationElement_ABC& population );
    //@}

private:
    //! @name Types
    //@{
    enum SendStateMode
    {
        eUpdate,
        eFull
    };
    //@}

private:
    //! @name Helpers
    //@{
    static MIL_BurningCellOrigin ComputeCellOriginFromPoint( double x, double y );
    MIL_BurningCell* FindCell( const MIL_BurningCellOrigin& coords ) const;
    void StartBurn( const MIL_BurningCellOrigin& cellOrigin, MIL_Object_ABC& object );
    void InitCell( const MIL_BurningCellOrigin& cellOrigin, MIL_Object_ABC& object, sword::EnumBurningCellPhase phase );
    void UpdatePreIgnition( MIL_BurningCell& cell, unsigned int time );
    void PropagateIgnition( const MIL_BurningCellOrigin& fromOrigin, MIL_BurningCell& to, unsigned int timeElapsed );
    void UpdateCombustion( MIL_BurningCell& cell );
    void UpdateDecline( MIL_BurningCell& cell );
    void FindTerrainData( const geometry::Point2d& center, float radius, TerrainData& data );

    void SendState( sword::ObjectAttributes& asn, MIL_Object_ABC& object, MIL_BurningCells::SendStateMode mode ) const;
    //@}

private:
    //! @name Member data
    //@{
    typedef std::map< MIL_BurningCellOrigin, MIL_BurningCell* > BurningCellsByCoordinatesMap;
    typedef std::vector< MIL_BurningCell* > BurningCellsVector;
    typedef std::map< unsigned int/*object id*/, BurningCellsVector > BurningCellsByObjectsMap;
    BurningCellsByCoordinatesMap burningCellsByCoordinates_;
    BurningCellsByObjectsMap burningCellsByObjects_;
    std::vector< MIL_Object_ABC* > propagationModifierObjects_;
    std::size_t lastCellIndexIncludedInLocalization_;
    //@}
};

#endif // __MIL_MIL_BurningCells_h_
