// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.inl $
// $Author: Jvt $
// $Modtime: 31/03/05 18:28 $
// $Revision: 5 $
// $Workfile: PHY_RolePion_Composantes.inl $
//
// *****************************************************************************

#include "MIL_AgentServer.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h" 

// -----------------------------------------------------------------------------
// Name: template< typename T > void PHY_RolePion_Composantes::Apply
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
template< typename T > 
inline void PHY_RolePion_Composantes::Apply( T& functor ) const
{
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
        functor( **it );
}

// -----------------------------------------------------------------------------
// Name: template< typename T > void PHY_RolePion_Composantes::GetComposante
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
template< typename T > 
inline PHY_ComposantePion* PHY_RolePion_Composantes::GetComposante( T& functor ) const
{
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        if( functor( **it ) )
            return *it;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: template< typename T > bool PHY_RolePion_Composantes::HasUsableComposante
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
template< typename T > 
inline bool PHY_RolePion_Composantes::HasUsableComposante( T& functor ) const
{
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        if( functor( ( **it).GetType() ) && (**it).GetState().IsUsable() )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: template< typename T > void PHY_RolePion_Composantes::GetComposantesUse
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
template< typename T > 
inline void PHY_RolePion_Composantes::GetComposantesUse( T_ComposanteUseMap& composanteUse, T& functor ) const
{
    composanteUse.clear();
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        const PHY_ComposantePion& comp = **it;
        if( functor( comp.GetType() ) )
        {
            T_ComposanteUse& data = composanteUse[ &comp.GetType() ];
            ++ data.nNbrTotal_;

            if( comp.GetState().IsUsable() )
            {
                ++ data.nNbrAvailable_;
                if( !functor( comp ) )
                    ++ data.nNbrUsed_;
            }
        }
    }

    for( CIT_LoanMap itLoan = lentComposantes_.begin(); itLoan != lentComposantes_.end(); ++itLoan )
    {
        const T_ComposantePionVector& composantes = itLoan->second;
        for( CIT_ComposantePionVector it = composantes.begin(); it != composantes.end(); ++it )
        {
            const PHY_ComposantePion& comp = **it;
            if( functor( comp.GetType() ) )
            {
                T_ComposanteUse& data = composanteUse[ &comp.GetType() ];
                ++ data.nNbrTotal_;
                ++ data.nNbrLent_;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::HasChanged
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Composantes::HasChanged() const
{
    return nNbrComposanteChanged_ != 0 || bLoansChanged_ || bOperationalStateChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::IsUsable
// Created: NLD 2005-08-09
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Composantes::IsUsable() const
{
    return nNbrUsableComposantes_ > 0 || !lentComposantes_.empty();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetMajorComposante
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
const PHY_ComposantePion* PHY_RolePion_Composantes::GetMajorComposante() const
{
    return pMajorComposante_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetOperationalState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
inline
MT_Float PHY_RolePion_Composantes::GetOperationalState() const
{
    return rOperationalState_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetMajorOperationalState
// Created: NLD 2005-11-25
// -----------------------------------------------------------------------------
inline
MT_Float PHY_RolePion_Composantes::GetMajorOperationalState() const
{
    return rMajorOperationalState_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetPion
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
inline
const MIL_AgentPion& PHY_RolePion_Composantes::GetPion() const
{
    assert( pPion_ );
    return *pPion_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::RepairAllComposantes
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Composantes::RepairAllComposantes()
{
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
        (**it).Repair();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::HealAllHumans
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Composantes::HealAllHumans()
{
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
        (**it).HealAllHumans();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::DestroyRandomComposante
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Composantes::DestroyRandomComposante()
{
    T_ComposantePionVector composantes = composantes_;
    std::random_shuffle( composantes.begin(), composantes.end() );

    for( CIT_ComposantePionVector it = composantes.begin(); it != composantes.end(); ++it )
    {
        PHY_ComposantePion& composante = **it;
        if( composante.GetState().IsUsable() )
        {
            composante.ReinitializeState( PHY_ComposanteState::dead_ );
            return;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::DestroyAllComposantes
// Created: NLD 2004-12-07
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Composantes::DestroyAllComposantes()
{
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
        (**it).ReinitializeState( PHY_ComposanteState::dead_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::IsNeutralized
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Composantes::IsNeutralized() const
{
    return nNeutralizationEndTimeStep_ >= MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
}

// =============================================================================
// LEND
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::LendComposantes
// Created: NLD 2006-04-03
// -----------------------------------------------------------------------------
template < typename T >
uint PHY_RolePion_Composantes::LendComposantes( PHY_RolePion_Composantes& borrower, uint nNbr, T funcPredicate )
{
    uint nNbrDone = 0;
    for( RIT_ComposantePionVector it = composantes_.rbegin(); it != composantes_.rend() && nNbrDone < nNbr ; )
    {
        PHY_ComposantePion& composante = **it;

        if( composante.CanBeLent() && funcPredicate( composante ) )
        {
            ++nNbrDone;
            LendComposante( borrower, composante );
            it = composantes_.rbegin(); // LendComposante->TransfertComposante modifie composantes_
        }
        else
            ++it;
    }
    return nNbrDone;
}

// -----------------------------------------------------------------------------
// Name: template < typename T > uint PHY_RolePion_Composantes::RetrieveLentComposantes
// Created: NLD 2006-04-04
// -----------------------------------------------------------------------------
template < typename T > 
uint PHY_RolePion_Composantes::RetrieveLentComposantes( PHY_RolePion_Composantes& borrower, uint nNbr, T funcPredicate )
{
    uint nNbrDone = 0;
    while( nNbrDone < nNbr )
    {
        IT_LoanMap it = lentComposantes_.find( &borrower );
        if( it == lentComposantes_.end() )
            return nNbrDone;

        PHY_ComposantePion* pComposante = 0;
        const T_ComposantePionVector& lentComps = it->second;
        for( CIT_ComposantePionVector it = lentComps.begin(); it != lentComps.end(); ++it )
        {
            PHY_ComposantePion& composante = **it;
            if( funcPredicate( composante ) )
                pComposante = &composante;
        }
        if( !pComposante )
            return nNbrDone;

        ++ nNbrDone;
        RetrieveLentComposante( borrower, *pComposante );
    }
    return nNbrDone;
}
   
// -----------------------------------------------------------------------------
// Name: template < typename T > uint PHY_RolePion_Composantes::GetLentComposantesTravelTime
// Created: NLD 2006-04-04
// -----------------------------------------------------------------------------
template < typename T > 
uint PHY_RolePion_Composantes::GetLentComposantesTravelTime( PHY_RolePion_Composantes& borrower, uint nNbr, T funcPredicate )
{
    uint nNbrDone = 0;
    uint nTime    = 0;

    const MT_Vector2D& srcPos  =          GetRole< PHY_RolePion_Location >().GetPosition();
    const MT_Vector2D& destPos = borrower.GetRole< PHY_RolePion_Location >().GetPosition();

    for( RIT_ComposantePionVector it = composantes_.rbegin(); it != composantes_.rend() && nNbrDone < nNbr; ++it )
    {
        PHY_ComposantePion& composante = **it;

        if( composante.CanBeLent() && funcPredicate( composante ) )
        {
            ++nNbrDone;
            nTime = std::max( nTime, composante.ApproximateTravelTime( srcPos, destPos ) );
        }
    }
    return nTime;
}
