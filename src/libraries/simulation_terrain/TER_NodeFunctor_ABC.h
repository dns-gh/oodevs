// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-01-31 $
// $Archive: /MVW_v10/Build/SDK/ter/src/TER_NodeFunctor_ABC.h $
// $Author: Age $
// $Modtime: 1/02/05 16:12 $
// $Revision: 1 $
// $Workfile: TER_NodeFunctor_ABC.h $
//
// *****************************************************************************

#ifndef __TER_NodeFunctor_ABC_h_
#define __TER_NodeFunctor_ABC_h_

#include "MT_Tools/MT_Vector2DTypes.h"

class TerrainData;

// =============================================================================
/** @class  TER_NodeFunctor_ABC
    @brief  TER_NodeFunctor_ABC
*/
// Created: AGE 2005-01-31
// =============================================================================
class TER_NodeFunctor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             TER_NodeFunctor_ABC() {}
    virtual ~TER_NodeFunctor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Visit( const MT_Vector2D& pos, const TerrainData& data ) = 0;
    //@}
};

// =============================================================================
/** @class  TER_NodeFunctor
    @brief  TER_NodeFunctor
*/
// Created: AGE 2005-01-31
// =============================================================================
template< typename F >
class TER_NodeFunctor : public TER_NodeFunctor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    TER_NodeFunctor( F& functor ) : functor_( functor ) {};
    //@}

    //! @name Operations
    //@{
    virtual void Visit( const MT_Vector2D& pos, const TerrainData& data )
    {
        return functor_.Visit( pos, data );
    };
    //@}
private:
    TER_NodeFunctor& operator=( const TER_NodeFunctor& );
    F& functor_;
};

template< typename F >
TER_NodeFunctor< F > CreateNodeFunctor( F& functor )
{
    return TER_NodeFunctor< F >( functor );
}

#endif // __TER_NodeFunctor_ABC_h_
