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

class PHY_ComposantePion;
class PHY_ComposanteTypePion;

// =============================================================================
// @class  PHY_ComposantePredicate
// Created: NLD 2006-08-02
// =============================================================================
class PHY_ComposantePredicate
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
class PHY_ComposantePredicate1
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
// @class  PHY_ComposanteTypePredicate
// Created: NLD 2006-08-02
// =============================================================================
class PHY_ComposanteTypePredicate
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
class PHY_ComposanteTypePredicate1
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
// @class  PHY_ComposanteUsePredicate
// Created: NLD 2006-08-02
// =============================================================================
class PHY_ComposanteUsePredicate
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
class PHY_ComposanteUsePredicate1
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

#endif // __PHY_ComposantePredicates_h_
