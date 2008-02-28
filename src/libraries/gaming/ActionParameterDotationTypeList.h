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

#include "ActionParameter.h"
#include "ActionParameterDotationType.h"

// =============================================================================
/** @class  ActionParameterDotationTypeList
    @brief  ActionParameterDotationTypeList
*/
// Created: AGE 2007-10-23
// =============================================================================
class ActionParameterDotationTypeList : public ActionParameter< std::string >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionParameterDotationTypeList( const kernel::OrderParameter& parameter );
    virtual ~ActionParameterDotationTypeList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( std::string& content ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterDotationTypeList( const ActionParameterDotationTypeList& );            //!< Copy constructor
    ActionParameterDotationTypeList& operator=( const ActionParameterDotationTypeList& ); //!< Assignment operator
    //@}
};

#endif // __ActionParameterDotationTypeList_h_
