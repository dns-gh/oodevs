// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Row_ABC_h_
#define __Row_ABC_h_

#include "FieldVariant.h"

namespace plugins
{
namespace esri
{
    class Shape_ABC;

// =============================================================================
/** @class  Row_ABC
    @brief  Row_ABC
*/
// Created: SBO 2007-08-30
// =============================================================================
class Row_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Row_ABC() {}
    virtual ~Row_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void SetField( const std::string& name, const FieldVariant& value ) = 0;
    virtual void SetShape( const Shape_ABC& value ) = 0;
    virtual FieldVariant GetField( const std::string& name ) const = 0;
    virtual Shape_ABC& GetShape() const = 0;
    virtual long GetID() const = 0;
    //@}
};

}
}

#endif // __Row_ABC_h_
