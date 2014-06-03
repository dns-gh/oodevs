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
             MIL_AutomateMission( const MIL_MissionType_ABC& type, MIL_Automate& automate, uint32_t id, const boost::shared_ptr< MIL_Mission_ABC >& parent );
             MIL_AutomateMission( const MIL_MissionType_ABC& type, MIL_Automate& automate, uint32_t id, const sword::MissionParameters& parameters );
             MIL_AutomateMission( MIL_Automate& automate, const MIL_AutomateMission& rhs, uint32_t id );
    virtual ~MIL_AutomateMission();
    //@}

    //! @name Accessors
    //@{
    MIL_Automate& GetAutomate() const;
    //@}

    //! @name Operations
    //@{
    virtual void                 Start               ( boost::shared_ptr< MIL_Mission_ABC > self, ActionManager& actions );
    virtual void                 Stop                ( boost::shared_ptr< MIL_Mission_ABC > self );
            void                 GoToCdt             ( boost::shared_ptr< MIL_Mission_ABC > self );
    virtual bool                 IsFragOrderAvailable( const MIL_FragOrderType& fragOrderType ) const;
    boost::shared_ptr< MIL_Mission_ABC > CreateCopy  ( MIL_Automate& target, uint32_t id ) const;
    virtual void                 AffectDirection     ( const MT_Vector2D& direction );

    virtual void Send( ActionManager& actions ) const;
    //@}

private:
    //! @name Tools
    //@{
    void StartMrtBehavior();
    void StopMrtBehavior ();
    void StartCdtBehavior();
    void StopCdtBehavior ();
    //@}

    //! @name Network
    //@{
    static void SendNoMission( const MIL_Automate& automate );
    //@}

private:
    //! @name Member data
    //@{
    MIL_Automate& automate_;
    bool          bDIAMrtBehaviorActivated_;
    bool          bDIACdtBehaviorActivated_;
    //@}
};

#endif // __MIL_AutomateMission_h_
