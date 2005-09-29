// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/PHY_Actor.inl $
// $Author: Jvt $
// $Modtime: 24/03/05 15:24 $
// $Revision: 5 $
// $Workfile: PHY_Actor.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_Actor::RegisterAction
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
inline
void PHY_Actor::RegisterAction( PHY_Action_ABC& action )
{
    bool bOut = actions_.insert( &action ).second;
    assert( bOut );
}

// -----------------------------------------------------------------------------
// Name: PHY_Actor::UnregisterAction
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
inline
void PHY_Actor::UnregisterAction( PHY_Action_ABC& action )
{
    int nOut = actions_.erase( &action );
    assert( nOut == 1 );
}

// -----------------------------------------------------------------------------
// Name: PHY_Actor::CancelAllActions
// Created: NLD 2005-03-18
// -----------------------------------------------------------------------------
inline
void PHY_Actor::CancelAllActions()
{
    actions_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_Actor::HasAction
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
inline
bool PHY_Actor::HasAction( PHY_Action_ABC& action ) const
{
    return actions_.find( &action ) != actions_.end();
}
