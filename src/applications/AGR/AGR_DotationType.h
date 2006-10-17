// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AGR_DotationType_h_
#define __AGR_DotationType_h_

#include "AGR_Type_ABC.h"

// =============================================================================
/** @class  AGR_DotationType
    @brief  AGR_DotationType
*/
// Created: AGE 2006-10-17
// =============================================================================
class AGR_DotationType : public AGR_Type_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_DotationType();
    virtual ~AGR_DotationType();
    //@}

    //! @name Operations
    //@{
    virtual std::string MOS2RCCode( const std::string& strAsnParam ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AGR_DotationType( const AGR_DotationType& );            //!< Copy constructor
    AGR_DotationType& operator=( const AGR_DotationType& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __AGR_DotationType_h_
