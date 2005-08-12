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
#include "Tools/Path.h"
#include "Tools/Location.h"
#include "Workspace.h"

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
class Pawn
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
          T_EntityId   GetId       () const;
	const std::string& GetName     () const;
    const PawnType&    GetType     () const;
	      bool	       IsPc        () const;
          T_Direction  GetDirection() const;
          T_Speed      GetSpeed    () const;
    const Position&    GetPosition () const;
          int          GetState    () const;
		  T_Height     GetHeight   () const;
    const Automat&     GetAutomat  () const;
    const Path&        GetPath     () const;
    //@}

    //! @name Messages handlers
    //@{
    void OnAutomatChanged    (       Automat&                 automat );
    void OnAttributeUpdated  ( const ASN1T_MsgUnitAttributes& asnMsg  );
    void OnAttributeUpdated  ( const ASN1T_MsgUnitDotations&  asnMsg  );
    void OnReceivePathfind   ( const ASN1T_MsgUnitPathFind&   asnMsg  );
    void OnReceiveTerrainType(       DIN::DIN_Input&          input   );
    //@}

    //! @name Mission Scheduling
    //@{
    void ScheduleAllMissions ( Scheduler& scheduler );
    void ScheduleMission     ( Scheduler& scheduler, const std::string& strMissionName );
    //@}

    //! @name Test Parameters
    //@{
    T_EntityId        GetTestParam_LeftLimit          ();
    T_EntityId        GetTestParam_RightLimit         ();
    T_IdVector&       GetTestParam_Limas              () const;

    T_EntityId        GetTestParam_ID                 () const;
    uint              GetTestParam_Direction          () const;
    bool              GetTestParam_Bool               () const;
    int               GetTestParam_Numeric            ( int  nMin = 0, int  nMax = std::numeric_limits< int >::max() ) const;
    uint              GetTestParam_Enumeration        ( uint nMin    , uint nMax            ) const;
    Position&         GetTestParam_Point              () const;
    T_PositionVector& GetTestParam_PointList          () const;
    Path&             GetTestParam_Path               () const;
    T_PathVector&     GetTestParam_PathList           () const;
    T_EntityId        GetTestParam_Agent              () const;
    T_IdVector&       GetTestParam_AgentList          () const;
    T_EntityId        GetTestParam_AgentKnowledge     () const;
    T_IdVector&       GetTestParam_AgentKnowledgeList () const;
    T_EntityId        GetTestParam_Automate           () const;
    T_IdVector&       GetTestParam_AutomateList       () const;
    T_EntityId        GetTestParam_ObjectKnowledge    () const;
    T_IdVector&       GetTestParam_ObjectKnowledgeList() const;
    Location&         GetTestParam_Location           () const;
    Location&         GetTestParam_Polygon            () const;
    T_LocationVector& GetTestParam_PolygonList        () const;
    uint              GetTestParam_NatureAtlas        () const;
    uint              GetTestParam_GDH                () const;
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
    T_EntityId       nLeftLimit_;
    T_EntityId       nRightLimit_;

    const Workspace& workspace_;
    //@}
};

} // end namespace TEST

#include "Pawn.inl"

#endif // __Pawn_h_
