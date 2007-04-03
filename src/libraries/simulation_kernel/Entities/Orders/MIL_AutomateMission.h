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

#include "MIL.h"

#include "MIL_Mission_ABC.h"

#include "MIL_AutomateMissionType.h"

class MIL_AgentPion;

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
             MIL_AutomateMission( const MIL_AutomateMissionType& type, MIL_Automate& automate, const ASN1T_MsgAutomateOrder& asn );
             MIL_AutomateMission( const MIL_AutomateMissionType& type, MIL_Automate& automate );
    virtual ~MIL_AutomateMission();
    //@}

    //! @name DIA
    //@{
    static void InitializeDIA();
    //@}

    //! @name Accessors
    //@{
    virtual const MIL_AutomateMissionType& GetType() const;
    //@}

    //! @name Operations
    //@{
    virtual void                 Start               ();
            void                 GoToCdt             ();
    virtual bool                 IsFragOrderAvailable( const MIL_FragOrderType& fragOrderType ) const;
            MIL_AutomateMission& CreateCopy          ( MIL_Automate& target ) const;
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
    void Send                  () const;
    void SendMsgOrderManagement( ASN1T_EnumOrderState nState ) const;
    //@}

private:
    const MIL_AutomateMissionType& type_;
          MIL_Automate&            automate_;
          bool                     bDIAMrtBehaviorActivated_;
          bool                     bDIACdtBehaviorActivated_;

private:
    static int nDIADirectionDanger_;
    static int nDIAFormation_;
};

#include "MIL_AutomateMission.inl"

#endif // __MIL_AutomateMission_h_
