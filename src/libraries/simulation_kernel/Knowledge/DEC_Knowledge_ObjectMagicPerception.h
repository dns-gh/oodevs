// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DEC_Knowledge_ObjectMagicPerception_h_
#define __DEC_Knowledge_ObjectMagicPerception_h_

#include "DEC_Knowledge_Object.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Objects/MIL_Object_ABC.h"

// =============================================================================
// Created: NLD 2010-10-26
// =============================================================================
class DEC_Knowledge_ObjectMagicPerception
{
public:
    DEC_Knowledge_ObjectMagicPerception( const DEC_Knowledge_Object& knowledge, const PHY_PerceptionLevel& perceptionLevel );
    ~DEC_Knowledge_ObjectMagicPerception();

    //! @name Main
    //@{
    template< typename T > const T* GetAttribute() const;
    //@}

private:
    const DEC_Knowledge_Object& knowledge_;
    const PHY_PerceptionLevel& perceptionLevel_;
};

// =============================================================================
// Implementation
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectMagicPerception::GetAttribute
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
template< typename T > const T* DEC_Knowledge_ObjectMagicPerception::GetAttribute() const
{
    const MIL_Object_ABC* pObject = knowledge_.GetObjectKnown();
    if( !pObject || perceptionLevel_ <= PHY_PerceptionLevel::notSeen_ )
        return 0;
 
    return pObject->RetrieveAttribute< T >();
}

#endif // __DEC_Knowledge_ObjectMagicPerception_h_
