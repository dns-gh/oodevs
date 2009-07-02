// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_AutomateMission_h_
#define __MIL_AutomateMission_h_

#include "MIL_Mission_ABC.h"
#include "MIL_MissionType_ABC.h"

class MIL_Automate;

// =============================================================================
/** @class  MIL_AutomateMission
    @brief  MIL_AutomateMission
*/
// Created: NLD 2006-11-14
// =============================================================================
class MIL_AutomateMission : public MIL_Mission_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_AutomateMission( const MIL_MissionType_ABC& type, MIL_Automate& automate, const ASN1T_MsgAutomatOrder& asn );
             MIL_AutomateMission( const MIL_MissionType_ABC& type, MIL_Automate& automate, const MIL_AutomateMission& parent );
             MIL_AutomateMission( const MIL_MissionType_ABC& type, MIL_Automate& automate );
    virtual ~MIL_AutomateMission();
    //@}

    //! @name DIA
    //@{
    static void InitializeDIA();
    //@}

    //! @name Accessors
    //@{
    MIL_Automate& GetAutomate() const;
    //@}

    //! @name Operations
    //@{
    virtual void                 Start               ();
            void                 GoToCdt             ();
    virtual bool                 IsFragOrderAvailable( const MIL_FragOrderType& fragOrderType ) const;
            MIL_AutomateMission& CreateCopy          ( MIL_Automate& target ) const;
    virtual void                 AffectDirection     ( const MT_Vector2D& direction );
    //@}

private:
    //! @name Constructors/Destructor
    //@{
    MIL_AutomateMission( MIL_Automate& automate, const MIL_AutomateMission& rhs );
    //@}

    //! @name Tools
    //@{
    void Stop();

    void StartMrtBehavior();
    void StopMrtBehavior ();
    void StartCdtBehavior();
    void StopCdtBehavior ();
    //@}

    //! @name Network
    //@{
           void Send         () const;
    static void SendNoMission( const MIL_Automate& automate );
    //@}

private:
          MIL_Automate&        automate_;
          bool                 bDIAMrtBehaviorActivated_;
          bool                 bDIACdtBehaviorActivated_;
};

#endif // __MIL_AutomateMission_h_
