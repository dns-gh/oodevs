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

// -----------------------------------------------------------------------------
// Name: template< typename T > void PHY_RolePion_Composantes::ApplyOnWeapons
// Created: NLD 2006-08-07
// -----------------------------------------------------------------------------
template< typename T > 
inline void PHY_RolePion_Composantes::ApplyOnWeapons( T& t ) const
{
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
        (**it).ApplyOnWeapons( t );
}

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
    // ne teste pas l'�tat logistique (= potentiellement utilisable)
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

    const MT_Vector2D& srcPos  =          pPion_->GetRole< PHY_RolePion_Location >().GetPosition();
    const MT_Vector2D& destPos = borrower.GetPion().GetRole< PHY_RolePion_Location >().GetPosition();   //@@Hmm...

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
