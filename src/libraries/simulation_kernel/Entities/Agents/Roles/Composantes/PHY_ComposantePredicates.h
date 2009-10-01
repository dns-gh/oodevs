// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h $
// $Author: Jvt $
// $Modtime: 12/05/05 15:16 $
// $Revision: 21 $
// $Workfile: PHY_RolePion_Composantes.h $
//
// *****************************************************************************

#ifndef __PHY_ComposantePredicates_h_
#define __PHY_ComposantePredicates_h_

#include "MIL.h"

#include "OnComponentFunctor_ABC.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"

class PHY_ComposantePion;
class PHY_ComposanteTypePion;

// =============================================================================
// @class  ComponentPredicate_ABC
// Created: MGD 2009-09-28
// =============================================================================
class ComponentPredicate_ABC
{
public:
    virtual bool operator() ( const PHY_ComposantePion& composante ) = 0;
};


// =============================================================================
// @class  PHY_ComposantePredicate
// Created: NLD 2006-08-02
// =============================================================================
class PHY_ComposantePredicate : public ComponentPredicate_ABC
{
public:
    //! @name Types
    //@{
    typedef bool (PHY_ComposantePion::*T_ComposantePredicate )() const;
    //@}

public:
    PHY_ComposantePredicate( T_ComposantePredicate compPredicate )
        : compPredicate_( compPredicate )
    {
    }

    bool operator() ( const PHY_ComposantePion& composante )
    {
        return (composante.*compPredicate_)();
    }

private:
    T_ComposantePredicate compPredicate_;
};

// =============================================================================
// @class  PHY_ComposanteUsePredicate1
// Created: NLD 2006-08-02
// =============================================================================
template< typename T >
class PHY_ComposantePredicate1 : public ComponentPredicate_ABC
{
public:
    //! @name Types
    //@{
    typedef bool (PHY_ComposantePion::*T_ComposantePredicate    )( const T& ) const;
    //@}

public:
    PHY_ComposantePredicate1< T >( T_ComposantePredicate compPredicate, const T& param )
        : compPredicate_( compPredicate )
        , param_        ( param )
    {
    }

    bool operator() ( const PHY_ComposantePion& composante )
    {
        return (composante.*compPredicate_)( param_ );
    }


private:
    const T&                    param_;
    const T_ComposantePredicate compPredicate_;
};

// =============================================================================
// @class  GetComponentFunctor //@TODO MGD Try to find a better design
// Created: MGD 2009-09-28
// =============================================================================
class GetComponentFunctor : public OnComponentFunctor_ABC
{

public:
    GetComponentFunctor( ComponentPredicate_ABC& functor )
        : functor_( functor )
        , result_( 0 )
    {
    }

    void operator() ( PHY_ComposantePion& composante )
    {
        if( functor_( composante ) )
            result_ = &composante;//@TODO MGD ADD optimisation
    }

    ComponentPredicate_ABC& functor_;
    PHY_ComposantePion* result_;
};


// =============================================================================
// @class  ComponentTypePredicate_ABC
// Created: MGD 2009-09-28
// =============================================================================
class ComponentTypePredicate_ABC
{
public:
    virtual bool operator() ( const PHY_ComposanteTypePion& composanteType ) = 0;
};

// =============================================================================
// @class  PHY_ComposanteTypePredicate
// Created: NLD 2006-08-02
// =============================================================================
class PHY_ComposanteTypePredicate : public ComponentTypePredicate_ABC
{
public:
    //! @name Types
    //@{
    typedef bool (PHY_ComposanteTypePion::*T_ComposanteTypePredicate)() const;
    //@}

public:
    PHY_ComposanteTypePredicate( T_ComposanteTypePredicate compTypePredicate )
        : compTypePredicate_( compTypePredicate )
    {
    }

    bool operator() ( const PHY_ComposanteTypePion& composanteType )
    {
        return (composanteType.*compTypePredicate_)();
    }

private:
    T_ComposanteTypePredicate compTypePredicate_;
};

// =============================================================================
// @class  PHY_ComposanteUsePredicate1
// Created: NLD 2006-08-02
// =============================================================================
template< typename T >
class PHY_ComposanteTypePredicate1 : public ComponentTypePredicate_ABC
{
public:
    //! @name Types
    //@{
    typedef bool (PHY_ComposanteTypePion::*T_ComposanteTypePredicate)( const T& ) const;
    //@}

public:
    PHY_ComposanteTypePredicate1< T >( T_ComposanteTypePredicate compTypePredicate, const T& param )
        : compTypePredicate_( compTypePredicate )
        , param_            ( param )
    {
    }

    bool operator() ( const PHY_ComposanteTypePion& composanteType )
    {
        return (composanteType.*compTypePredicate_)( param_ );
    }

private:
    const T&                        param_;
    const T_ComposanteTypePredicate compTypePredicate_;
};

// =============================================================================
// @class  HasUsableComponentFunctor
// Created: MGD 2009-09-28
// =============================================================================
class HasUsableComponentFunctor : public OnComponentFunctor_ABC
{

public:
    HasUsableComponentFunctor( ComponentTypePredicate_ABC& functor )
        : functor_( functor )
        , result_( false )
    {
    }

    void operator() ( PHY_ComposantePion& composante )
    {
        if( functor_( composante.GetType() ) && composante.GetState().IsUsable() )
            result_ = true;//@TODO MGD ADD optimisation
    }

    ComponentTypePredicate_ABC& functor_;
    bool result_;
};

// =============================================================================
// @class  ComposanteUsePredicate_ABC
// Created: MGD 2009-09-28
// =============================================================================
class ComposanteUsePredicate_ABC
{
public:
    virtual bool operator() ( const PHY_ComposantePion& composante ) = 0;
    virtual bool operator() ( const PHY_ComposanteTypePion& composanteType ) = 0;
};

// =============================================================================
// @class  PHY_ComposanteUsePredicate
// Created: NLD 2006-08-02
// =============================================================================
class PHY_ComposanteUsePredicate : public ComposanteUsePredicate_ABC
{
public:
    //! @name Types
    //@{
    typedef bool (PHY_ComposantePion    ::*T_ComposantePredicate    )() const;
    typedef bool (PHY_ComposanteTypePion::*T_ComposanteTypePredicate)() const;
    //@}

public:
    PHY_ComposanteUsePredicate( T_ComposantePredicate compPredicate, T_ComposanteTypePredicate compTypePredicate )
        : compPredicate_    ( compPredicate )
        , compTypePredicate_( compTypePredicate )
    {
    }

    bool operator() ( const PHY_ComposantePion& composante )
    {
        return (composante.*compPredicate_)();
    }

    bool operator() ( const PHY_ComposanteTypePion& composanteType )
    {
        return (composanteType.*compTypePredicate_)();
    }

private:
    T_ComposantePredicate     compPredicate_;
    T_ComposanteTypePredicate compTypePredicate_;
};

// =============================================================================
// @class  PHY_ComposanteUsePredicate1
// Created: NLD 2006-08-02
// =============================================================================
template< typename T >
class PHY_ComposanteUsePredicate1 : public ComposanteUsePredicate_ABC
{
public:
    //! @name Types
    //@{
    typedef bool (PHY_ComposantePion    ::*T_ComposantePredicate    )( const T& ) const;
    typedef bool (PHY_ComposanteTypePion::*T_ComposanteTypePredicate)( const T& ) const;
    //@}

public:
    PHY_ComposanteUsePredicate1< T >( T_ComposantePredicate compPredicate, T_ComposanteTypePredicate compTypePredicate, const T& param )
        : compPredicate_    ( compPredicate )
        , compTypePredicate_( compTypePredicate )
        , param_            ( param )
    {
    }

    bool operator() ( const PHY_ComposantePion& composante )
    {
        return (composante.*compPredicate_)( param_ );
    }

    bool operator() ( const PHY_ComposanteTypePion& composanteType )
    {
        return (composanteType.*compTypePredicate_)( param_ );
    }

private:
    const T&                        param_;
    const T_ComposantePredicate     compPredicate_;
    const T_ComposanteTypePredicate compTypePredicate_;
};

// =============================================================================
// @class  GetComponentUseFunctor
// Created: MGD 2009-09-28
// =============================================================================
class GetComponentUseFunctor : public OnComponentFunctor_ABC
{

public:
    GetComponentUseFunctor( ComposanteUsePredicate_ABC& functor, PHY_Composante_ABC::T_ComposanteUseMap& result )
        : functor_( functor )
        , result_( result )
    {
    }

    void operator() ( PHY_ComposantePion& composante )
    {
        if( functor_( composante.GetType() ) )
        {
            PHY_Composante_ABC::T_ComposanteUse& data = result_[ &composante.GetType() ];
            ++ data.nNbrTotal_;

            if( composante.GetState().IsUsable() )
            {
                ++ data.nNbrAvailable_;
                if( !functor_( composante ) )
                    ++ data.nNbrUsed_;
            }
        }
    }

    ComposanteUsePredicate_ABC& functor_;
    PHY_Composante_ABC::T_ComposanteUseMap& result_;
};

// =============================================================================
// @class  HasUsableComponentFunctor
// Created: MGD 2009-09-28
// =============================================================================
class GetComponentLendedUseFunctor : public OnComponentFunctor_ABC
{

public:
    GetComponentLendedUseFunctor( ComposanteUsePredicate_ABC& functor, PHY_Composante_ABC::T_ComposanteUseMap& result )
        : functor_( functor )
        , result_( result )
    {
    }

    void operator() ( PHY_ComposantePion& composante )
    {
        if( functor_( composante.GetType() ) )
        {
            PHY_Composante_ABC::T_ComposanteUse& data = result_[ &composante.GetType() ];
            ++ data.nNbrTotal_;
            ++ data.nNbrLent_;
        }
    }

    ComposanteUsePredicate_ABC& functor_;
    PHY_Composante_ABC::T_ComposanteUseMap& result_;
};

#endif // __PHY_ComposantePredicates_h_
