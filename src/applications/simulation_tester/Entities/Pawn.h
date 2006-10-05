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
#include "Messages/ASN_Messages.h"
#include "Testable_Entity.h"

namespace DIN
{
    class DIN_Input;
}

namespace TEST
{
    class Automat;
    class PawnType;
    class Workspace;

// =============================================================================
// Created: SBO 2005-05-11
// =============================================================================
class Pawn : public Testable_Entity
{

public:
    //! @name Constructors/Destructor
    //@{
             Pawn( const Workspace& workspace, const ASN1T_MsgPionCreation& asnMsg );
             Pawn( const Workspace& workspace, const ASN1T_MsgAutomateCreation& asnMsg, Automat& automat  );
    virtual ~Pawn();
    //@}

    //! @name Accessors
    //@{
    virtual       T_EntityId   GetId       () const;
    virtual const Position&    GetPosition () const;
    virtual const Automat*     GetAutomat  () const;

	        const std::string& GetName     () const;
            const PawnType&    GetType     () const;
                  bool         IsPc        () const;
                  T_Direction  GetDirection() const;
                  T_Speed      GetSpeed    () const;
                  int          GetState    () const;
		          T_Height     GetHeight   () const;
            const Path&        GetPath     () const;
    //@}

    //! @name Messages handlers
    //@{
    void OnAutomatChanged    (       Automat&                 automat );
    void OnAttributeUpdated  ( const ASN1T_MsgUnitAttributes& asnMsg  );
    void OnReceivePathfind   ( const ASN1T_MsgUnitPathFind&   asnMsg  );
    void OnReceiveTerrainType(       DIN::DIN_Input&          input   );
    //@}

    //! @name Operations
    //@{
    void SendMagicAction( int action ) const;
    //@}

    //! @name Mission Scheduling
    //@{
    void ScheduleAllMissions ( Scheduler& scheduler, uint nIteration = 1 );
    void ScheduleMission     ( Scheduler& scheduler, const std::string& strMissionName, uint nIteration = 1 );
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
    T_Direction      nDirection_;
    T_Speed          rSpeed_;
    Position         position_;
    T_Height         nHeight_;
    int              nOpState_;
    Path             path_;
    bool             bIsLoaded_;
    //@}
};

} // end namespace TEST

#include "Pawn.inl"

#endif // __Pawn_h_
