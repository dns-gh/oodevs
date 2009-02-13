// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Row_h_
#define __Row_h_

#include "FieldVariant.h"
#include "Row_ABC.h"
#include "ESRI.h"

namespace plugins
{
namespace crossbow
{
    class Shape_ABC;

// =============================================================================
/** @class  Row
    @brief  Row
*/
// Created: SBO 2007-08-30
// =============================================================================
class Row : public Row_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Row();
    virtual ~Row();
    //@}

    //! @name Operations
    //@{
    virtual void SetField( const std::string& name, const FieldVariant& value );
    virtual void SetShape( const Shape_ABC& value );
    virtual FieldVariant GetField( const std::string& name ) const;
    virtual Shape_ABC& GetShape() const;
    virtual long GetID() const;

    void BindRow( IRowPtr row );
    void Commit( ICursorPtr cursor = NULL );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Row( const Row& );            //!< Copy constructor
    Row& operator=( const Row& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    long GetFieldIndex( const std::string& name ) const;
    //@}

private:
    //! @name Member data
    //@{
    IRowPtr row_;
    //@}
};

}
}

#endif // __Row_h_
