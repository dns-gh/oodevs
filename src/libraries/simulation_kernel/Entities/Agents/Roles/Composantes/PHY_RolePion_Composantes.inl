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

#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"

// -----------------------------------------------------------------------------
// Name: template< typename T > void PHY_RolePion_Composantes::ApplyOnWeapons
// Created: NLD 2006-08-07
// -----------------------------------------------------------------------------
template< typename T > 
inline void PHY_RolePion_Composantes::ApplyOnWeapons( T& t ) const
{
    for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
        ( **it ).ApplyOnWeapons( t );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::LendComposantes
// Created: NLD 2006-04-03
// -----------------------------------------------------------------------------
template < typename T >
unsigned int PHY_RolePion_Composantes::LendComposantes( MIL_Agent_ABC& borrower, unsigned int nNbr, T funcPredicate )
{
    unsigned int nNbrDone = 0;
    for( PHY_ComposantePion::RIT_ComposantePionVector it = composantes_.rbegin(); it != composantes_.rend() && nNbrDone < nNbr ; )
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
// Name: template < typename T > unsigned int PHY_RolePion_Composantes::RetrieveLentComposantes
// Created: NLD 2006-04-04
// -----------------------------------------------------------------------------
template < typename T > 
unsigned int PHY_RolePion_Composantes::RetrieveLentComposantes( MIL_Agent_ABC& borrower, unsigned int nNbr, T funcPredicate )
{
    unsigned int nNbrDone = 0;
    while( nNbrDone < nNbr )
    {
        IT_LoanMap it = lentComposantes_.find( &borrower );
        if( it == lentComposantes_.end() )
            return nNbrDone;
        PHY_ComposantePion* pComposante = 0;
        const PHY_ComposantePion::T_ComposantePionVector& lentComps = it->second;
        for( PHY_ComposantePion::CIT_ComposantePionVector it = lentComps.begin(); it != lentComps.end(); ++it )
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
// Name: template < typename T > unsigned int PHY_RolePion_Composantes::GetLentComposantesTravelTime
// Created: NLD 2006-04-04
// -----------------------------------------------------------------------------
template < typename T > 
unsigned int PHY_RolePion_Composantes::GetLentComposantesTravelTime( MIL_Agent_ABC& borrower, unsigned int nNbr, T funcPredicate )
{
    unsigned int nNbrDone = 0;
    unsigned int nTime = 0;
    const MT_Vector2D& srcPos = owner_.GetRole< PHY_RoleInterface_Location >().GetPosition();
    const MT_Vector2D& destPos = borrower.GetRole< PHY_RoleInterface_Location >().GetPosition();   //@@Hmm...
    for( PHY_ComposantePion::RIT_ComposantePionVector it = composantes_.rbegin(); it != composantes_.rend() && nNbrDone < nNbr; ++it )
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
