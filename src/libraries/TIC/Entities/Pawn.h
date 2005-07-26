// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-11 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/entities/Pawn.h $
// $Author: Sbo $
// $Modtime: 21/07/05 17:05 $
// $Revision: 24 $
// $Workfile: Pawn.h $
//
// *****************************************************************************

#ifndef __Pawn_h_
#define __Pawn_h_

#include "Types.h"
#include "Entities/ConcreteEntity.h"
#include "Messages/ASN_Messages.h"
#include "pathfind/TerrainData.h"
#include "Tools/Path.h"

namespace DIN
{
    class DIN_Input;
}

namespace TIC
{
    class Automat;
    class Platform;
    class PawnType;
    class PlatformType;
    class NatureLevel;
    class NatureWeapon;
    class NatureSpecialization;
    class NatureCategory;
    class NatureMobility;
    class NatureQualifier;
    class NatureCapacity;

    class Formation_ABC;

// =============================================================================
/** @class  Pawn
    @brief  Pawn
    @par    Using example
    @code
    Pawn;
    @endcode
*/
// Created: SBO 2005-05-11
// =============================================================================
class Pawn : public ConcreteEntity
{

public:
    //! @name Constructors/Destructor
    //@{
             Pawn( const ASN1T_MsgPionCreation& asnMsg );
             Pawn( const ASN1T_MsgAutomateCreation& asnMsg, Automat& automat  );
    virtual ~Pawn();
    //@}

    //! @name Static Operations
    //@{
    static void  Initialize             ();
    static void  Terminate              ();
    static Pawn* Find                   ( T_EntityId nId  );
    static void  Register               ( Pawn&      pawn );
    static void  Unregister             ( Pawn&      pawn );
    static void  AddDisaggregation      ( T_EntityId nId  );
    static void  UpdatePlatformPositions();
    //@}

    //! @name Facade Accessors
    //@{
          T_EntityId            GetId                  () const;
	const std::string&          GetName                () const;
    const std::string&          GetType                () const;
    const NatureLevel&          GetNatureLevel         () const;
    const NatureWeapon&         GetNatureWeapon        () const;
    const NatureSpecialization& GetNatureSpecialization() const;
    const NatureCategory&       GetNatureCategory      () const;
    const NatureMobility&       GetNatureMobility      () const;
    const NatureQualifier&      GetNatureQualifier     () const;
    const NatureCapacity&       GetNatureCapacity      () const;

	      bool	                IsPc                   () const;
          bool                  IsAggregated           () const;
          uint                  GetPlatformCount       () const;
    const Platform*             GetPlatformAt          ( uint nIdx ) const;
    //@}

    //! @name Other Accessors
    //@{
    const Automat& GetAutomat() const;
          bool     IsLoaded  () const;
    const Path&    GetPath   () const;
    //@}

    //! @name Modifiers
    //@{
    void OnAutomatChanged    (       Automat&                 automat );
    void OnAttributeUpdated  ( const ASN1T_MsgUnitAttributes& asnMsg  );
    void OnAttributeUpdated  ( const ASN1T_MsgUnitDotations&  asnMsg  );
    void OnReceivePathfind   ( const ASN1T_MsgUnitPathFind&   asnMsg  );
    void OnReceiveTerrainType(       DIN::DIN_Input&          input   );
    //@}

private:
    //! @name types
    //@{
    typedef std::map< T_EntityId, Pawn* >	         T_PawnMap;
	typedef T_PawnMap::const_iterator		         CIT_PawnMap;

    typedef std::set< T_EntityId >                   T_EntityIdSet;
    typedef T_EntityIdSet::const_iterator            CIT_EntityIdSet;

    typedef std::vector< Platform* >                 T_PlatformVector;
    typedef T_PlatformVector::iterator               IT_PlatformVector;
    typedef T_PlatformVector::const_iterator         CIT_PlatformVector;
    typedef T_PlatformVector::const_reverse_iterator CRIT_PlatformVector;

    typedef std::map< Platform*, Pawn* >             T_LendMap;
    typedef T_LendMap::iterator                      IT_LendMap;
    typedef T_LendMap::const_iterator                CIT_LendMap;

    typedef std::vector< Pawn* >                     T_PawnVector;
    typedef T_PawnVector::const_iterator             CIT_PawnVector;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Pawn( const Pawn& );            //!< Copy constructor
    Pawn& operator=( const Pawn& ); //!< Assignement operator
    //@}

    //! @name Aggregation management
    //@{
    void Aggregate   ();
	void Disaggregate();
    //@}

    //! @name Accessors
    //@{
    bool MustBeDisaggregated() const;
    //@}

    //! @name Internal platform management
    //@{
    void UpdateDisaggregationStatus();
    void UpdatePlatformPosition    () const;
    bool LendPlatform              ( Pawn& receiver, const PlatformType& platformType, uint nNbr, T_LendMap& previousLends );
    void SetFormation              ( Formation_ABC& formation ) const;
	//@}

    //! @name Debug
    //@{
    void SendPlatformPositionToMos( bool bClear ) const;
    //@}

private:
    //! @name Member data
    //@{
    T_EntityId            nId_;
	std::string 		  strName_;
	bool                  bIsPc_;
    const PawnType*       pType_;
	T_PlatformVector      childPlatforms_;
    Automat*              pAutomat_;

    // embedding, reinforcement, transport, lends... 
    bool                  bIsLoaded_;
    bool                  bIsTransported_;
    T_PawnVector          transportedPawns_;
    const Pawn*           pReinforcedPawn_;
    T_PawnVector          reinforcingPawns_;
    T_LendMap             lends_;

    // disaggregation
	bool                  bIsAggregated_;
    bool                  bConfigDisaggreg_; // if disaggregation has been specified in config file
    
    // terrain type
    TerrainData           terrain_;
    bool                  bIsOnRoad_;
    Path                  path_;
    //@}

private:
    //! @name Static members
    //@{
    // pawn list
    static T_PawnMap      pawns_;
    static T_EntityIdSet  disaggregPawns_; // pawn specified to be disaggregated in config file

    // road terrain
    static TerrainData    roadTerrain_;
    //@}
};

} // end namespace TIC

#include "Pawn.inl"

#endif // __Pawn_h_
