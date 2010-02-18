// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __ChildObjectAttribute_h_
#define __ChildObjectAttribute_h_

#include "ObjectAttribute_ABC.h"

// =============================================================================
/** @class  DetectorAttribute
@brief  DetectorAttribute
*/
// Created: SLG 2010-02-16
// =============================================================================
class ChildObjectAttribute : public ObjectAttribute_ABC
{ 

public:
    //! @name Constructors/Destructor
    //@{
    ChildObjectAttribute();
    explicit ChildObjectAttribute( Object& object );
    virtual ~ChildObjectAttribute();
    //@}

    //! @name CheckPoint
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Operations
    //@{
    void AddChildObject( Object& object );
    Object* GetChildObject() const;
    //@}

private:
    //! @name Member data
    //@{
    Object*    childObject_;
    //@}
};

#endif // __ChildObjectAttribute_h_
