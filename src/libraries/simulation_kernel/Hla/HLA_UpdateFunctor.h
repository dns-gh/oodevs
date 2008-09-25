// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-11-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Hla/HLA_UpdateFunctor.h $
// $Author: Age $
// $Modtime: 13/12/04 9:59 $
// $Revision: 2 $
// $Workfile: HLA_UpdateFunctor.h $
//
// *****************************************************************************

#ifndef __HLA_UpdateFunctor_h_
#define __HLA_UpdateFunctor_h_

#include <hla/UpdateFunctor_ABC.h>
#include <hla/AttributeIdentifier.h>
#include <hla/Serializer.h>
#include "HLA_Serialization.h"

// =============================================================================
/** @class  HLA_UpdateFunctor
    @brief  HLA update functor
*/
// Created: AGE 2004-11-08
// =============================================================================
class HLA_UpdateFunctor
{

public:
    //! @name Constructors/Destructor
    //@{
             HLA_UpdateFunctor( hla::UpdateFunctor_ABC& functor, bool bUpdateAll );
    virtual ~HLA_UpdateFunctor();
    //@}

    //! @name Operations
    //@{
    template< typename Type >
    void Serialize( const std::string& strAttributeName, bool bChanged, const Type& attribute );
    //@}

    //! @name Accessors
    //@{
    bool MustUpdateAll() const; //!< For performance issues
    //@}

private:
    //! @name Copy/Assignement
    //@{
    HLA_UpdateFunctor( const HLA_UpdateFunctor& );            //!< Copy constructor
    HLA_UpdateFunctor& operator=( const HLA_UpdateFunctor& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    hla::UpdateFunctor_ABC& functor_;
    bool bUpdateAll_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: HLA_UpdateFunctor::Serialize
// Created: AGE 2004-11-08
// -----------------------------------------------------------------------------
template< typename Type >
void HLA_UpdateFunctor::Serialize( const std::string& strAttributeName, bool bChanged, const Type& attribute )
{
    if( bChanged || bUpdateAll_ )
    {
        hla::Serializer serializer;
        serializer << attribute;
        functor_.Visit( hla::AttributeIdentifier( strAttributeName ), serializer );
    }
}

#endif // __HLA_UpdateFunctor_h_
