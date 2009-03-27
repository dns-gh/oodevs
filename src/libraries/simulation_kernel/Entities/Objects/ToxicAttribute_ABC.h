// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __ToxicAttribute_ABC_h_
#define __ToxicAttribute_ABC_h_

#include "ObjectAttribute_ABC.h"

class MIL_ToxicEffectManipulator;
class NBCAttribute;

// =============================================================================
/** @class  PropagationAttribute_ABC
    @brief  PropagationAttribute_ABC
*/
// Created: JCR 2008-05-30
// =============================================================================
class ToxicAttribute_ABC : public ObjectAttribute_ABC
{ 
public:
    //! @name Constructors/Destructor
    //@{
            ToxicAttribute_ABC() {}
    virtual ~ToxicAttribute_ABC() {}
    //@}

    //! @name 
    //@{
    virtual MIL_ToxicEffectManipulator GetContaminationEffect( const NBCAttribute& nbc, const MT_Vector2D& position ) const = 0;    
    //@}
};

#endif // __ToxicAttribute_ABC_h_
