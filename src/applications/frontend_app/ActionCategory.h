// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionCategory_h_
#define __ActionCategory_h_

#include <qvbuttongroup.h>

// =============================================================================
/** @class  ActionCategory
    @brief  ActionCategory
*/
// Created: SBO 2007-10-04
// =============================================================================
class ActionCategory : public QVButtonGroup
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionCategory( QWidget* parent );
    virtual ~ActionCategory();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionCategory( const ActionCategory& );            //!< Copy constructor
    ActionCategory& operator=( const ActionCategory& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __ActionCategory_h_
