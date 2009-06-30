// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/PHY_Actor.h $
// $Author: Jvt $
// $Modtime: 31/03/05 11:38 $
// $Revision: 18 $
// $Workfile: PHY_Actor.h $
//
// *****************************************************************************

#ifndef __PHY_Actor_h_
#define __PHY_Actor_h_

class PHY_Action_ABC;

// =============================================================================
// @class  PHY_Actor
// Created: JVT 2004-08-03
// =============================================================================
class PHY_Actor
{
    MT_COPYNOTALLOWED( PHY_Actor )

public:
             PHY_Actor();
    virtual ~PHY_Actor();

    //! @name CheckPoints
    //@{
    template< typename Archive > // Actions are not serialized
    void serialize( Archive&, const uint ) {}
    //@}

    //! @name Operations
    //@{
    void UpdateActions();
    //@}

    //! @name Actions
    //@{
    void RegisterAction  ( PHY_Action_ABC& action );
    void UnregisterAction( PHY_Action_ABC& action );
    void CancelAllActions();
    bool HasAction       ( PHY_Action_ABC& action ) const;
    //@}

private:
    std::set< PHY_Action_ABC* > actions_;
};

#endif // __PHY_Actor_h_
