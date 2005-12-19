// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/ConvoySupply/PHY_ActionConvoySupply.inl $
// $Author: Nld $
// $Modtime: 19/11/04 12:02 $
// $Revision: 1 $
// $Workfile: PHY_ActionConvoySupply.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_ActionLogistic constructor
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
template< typename T >
PHY_ActionLogistic< T >::PHY_ActionLogistic( T& entity )
    : PHY_Action_ABC( entity )
    , entity_       ( entity )
{

}

// -----------------------------------------------------------------------------
// Name: PHY_ActionLogistic destructor
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
template< typename T >
PHY_ActionLogistic< T >::~PHY_ActionLogistic()
{

}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ActionLogistic::Execute
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
template< typename T >
void PHY_ActionLogistic< T >::Execute()
{
    entity_.UpdateLogistic();
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionLogistic::ExecuteSuspended
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
template< typename T >
void PHY_ActionLogistic< T >::ExecuteSuspended()
{
    Execute();
}

