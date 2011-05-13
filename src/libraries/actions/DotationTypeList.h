// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterDotationTypeList_h_
#define __ActionParameterDotationTypeList_h_

#include "Parameter.h"
#include "DotationType.h"

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  DotationTypeList
    @brief  DotationTypeList
*/
// Created: AGE 2007-10-23
// =============================================================================
class DotationTypeList : public Parameter< std::string >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DotationTypeList( const kernel::OrderParameter& parameter );
    virtual ~DotationTypeList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( std::string& content ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual std::string SerializeType() const;
    //@}

    //! @name Copy/Assignment
    //@{
    DotationTypeList( const DotationTypeList& );            //!< Copy constructor
    DotationTypeList& operator=( const DotationTypeList& ); //!< Assignment operator
    //@}
};

    }
}

#endif // __ActionParameterDotationTypeList_h_
