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

#include <map>
#include <geometry/types.h>

class MIL_Object_ABC;
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

namespace sword
{
	enum EnumBurningCellPhase;
	class ObjectAttributes;
}

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
	void Update( MIL_Object_ABC& object, unsigned int time );
    //@}

	//! @name CheckPoints
    //@{
    void load( MIL_CheckPointInArchive&, MIL_Object_ABC&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, MIL_Object_ABC&, const unsigned int ) const;
    //@}

	//! @name Dispatch & save
    //@{
	void SendFullState( sword::ObjectAttributes& asn, MIL_Object_ABC& object ) const;
	void SendUpdate( sword::ObjectAttributes& asn, MIL_Object_ABC& object ) const;
	void WriteODB( xml::xostream& xos, MIL_Object_ABC& ) const;
	//@}
private:
	//! @name Types
    //@{
	typedef geometry::Point2< int > BurningCellOrigin;

	// $$$$ BCI 2011-01-04: todo faire plusieurs classes en fonction des phases...
	class BurningCell : private boost::noncopyable
	{
	public:
		explicit BurningCell( const BurningCellOrigin& origin )
			: origin_( origin )
		{
			//NOTHING
		}

		const BurningCellOrigin origin_;
		geometry::Point2d center_;
		MIL_Object_ABC* pObject_;
		sword::EnumBurningCellPhase phase_;
		int ignitionThreshold_;
		int maxCombustionEnergy_;
		int ignitionEnergy_;
		int combustionEnergySum_;
		int combustionEnergyCount_;
		int currentHeat_;
		int currentCombustionEnergy_;
		unsigned int lastUpdateTime_;
		bool bUpdated_;
	};

	enum SendStateMode
	{
		eUpdate,
		eFull
	};
    
	//! @name Helpers
    //@{
	static BurningCellOrigin ComputeCellOriginFromPoint( double x, double y, int cellSize );
	BurningCell* FindCell( const BurningCellOrigin& coords ) const;
    void StartBurn( const BurningCellOrigin& cellOrigin, MIL_Object_ABC& object );
	void InitCell( const BurningCellOrigin& cellOrigin, MIL_Object_ABC& object, sword::EnumBurningCellPhase phase );
	void UpdatePreIgnition( BurningCell& cell, unsigned int time );
	void PropagateIgnition( const BurningCellOrigin& fromOrigin, BurningCell& to, unsigned int timeElapsed );
	void UpdateCombustion( BurningCell& cell );
	void UpdateDecline( BurningCell& cell );


	template< SendStateMode mode > void SendState( sword::ObjectAttributes& asn, MIL_Object_ABC& object ) const;
	//@}

private:
	typedef std::map< BurningCellOrigin, BurningCell* > BurningCellsByCoordinatesMap;
	BurningCellsByCoordinatesMap burningCellsByCoordinates_;
	typedef std::vector< BurningCell* > BurningCellsVector;
	typedef std::map< MIL_Object_ABC*, BurningCellsVector > BurningCellsByObjectsMap;
	BurningCellsByObjectsMap burningCellsByObjects_;
    std::size_t lastCellIndexIncludedInLocalization_;
};

#endif // __MIL_BurningCells_h_
