// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-12-06 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Hla/HLA_ObjectInteraction.h $
// $Author: Age $
// $Modtime: 6/12/04 14:54 $
// $Revision: 1 $
// $Workfile: HLA_ObjectInteraction.h $
//
// *****************************************************************************

#ifndef __HLA_ObjectInteraction_h_
#define __HLA_ObjectInteraction_h_

#include <hla/ObjectIdentifier.h>

namespace hla
{
template < typename T > class Interaction;
}

class MIL_Object_ABC;
class HLA_Object_ABC;

// =============================================================================
/** @class  HLA_ObjectInteraction
    @brief  HLA object interaction helper
*/
// Created: AGE 2004-12-06
// =============================================================================
class HLA_ObjectInteraction
{

public:
    //! @name Constructors/Destructor
    //@{
             HLA_ObjectInteraction();
             HLA_ObjectInteraction( const HLA_Object_ABC& target );
    virtual ~HLA_ObjectInteraction();
    //@}

protected:
    //! @name Operations
    //@{
    MIL_Object_ABC* GetObject() const;

    template< typename SubClass >
    static void RegisterAttributes( hla::Interaction< SubClass >& interaction );
    //@}

private:
    //! @name Member data
    //@{
    hla::ObjectIdentifier objectId_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: HLA_ObjectInteraction::RegisterAttributes
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
template< typename SubClass >
void HLA_ObjectInteraction::RegisterAttributes( hla::Interaction< SubClass >& interaction )
{
    interaction.Register( hla::ParameterIdentifier( "objet" ), *new hla::Encodable< SubClass, ObjectIdentifier >( & HLA_ObjectInteraction::objectId_ ) );
}

#endif // __HLA_ObjectInteraction_h_
