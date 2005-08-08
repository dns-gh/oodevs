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
#include "Tools/Path.h"

namespace DIN
{
    class DIN_Input;
}

namespace TEST
{
    class Automat;
    class PawnType;

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
    static void  Initialize();
    static void  Terminate ();
    static Pawn* Find      ( T_EntityId nId  );
    static void  Register  ( Pawn&      pawn );
    static void  Unregister( Pawn&      pawn );
    //@}

    //! @name Facade Accessors
    //@{
          T_EntityId   GetId     () const;
	const std::string& GetName   () const;
    const std::string& GetType   () const;
	      bool	       IsPc      () const;
    //@}

    //! @name Test Parameters
    //@{
    Position&   GetTP_Position         () const;
    uint        GetTP_LeftLimit        () const;
    uint        GetTP_RightLimit       () const;
    T_IdVector& GetTP_Limas            () const;
    uint        GetTP_DangerDirection  () const;
    Path&       GetTP_Path             () const;
    uint        GetTP_PathType         () const;
    bool        GetTP_IsUnLoaded       () const;
    uint        GetTP_VerouillageVision() const;
    Position&   GetTP_VisionPoint      () const;
    uint        GetTP_Direction        () const;
    T_IdVector& GetTP_PawnList         () const;
    uint        GetTP_ObjectType       () const;
    bool        GetTP_IsIndirectFire   () const;
    uint        GetTP_MunitionType     () const;
    uint        GetTP_NbObus           () const;
    T_IdVector& GetTP_Targets          () const;
    //@}

    //! @name Other Accessors
    //@{
    const Automat&     GetAutomat() const;
    const Path&        GetPath   () const;
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
    typedef std::map< T_EntityId, Pawn* > T_PawnMap;
	typedef T_PawnMap::const_iterator     CIT_PawnMap;

    typedef std::set< Pawn* >             T_PawnSet;
	typedef T_PawnSet::const_iterator     CIT_PawnSet;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Pawn( const Pawn& );            //!< Copy constructor
    Pawn& operator=( const Pawn& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    T_EntityId       nId_;
	std::string      strName_;
	bool             bIsPc_;
    const PawnType*  pType_;
    Automat*         pAutomat_;

    Path             path_;

    bool             bIsLoaded_;
    //@}

private:
    //! @name Static members
    //@{
    // pawn list
    static T_PawnMap pawns_;
    //@}
};

} // end namespace TEST

#include "Pawn.inl"

#endif // __Pawn_h_
