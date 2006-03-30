// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticToolbar_h_
#define __LogisticToolbar_h_

class Controllers;

// =============================================================================
/** @class  LogisticToolbar
    @brief  LogisticToolbar
*/
// Created: AGE 2006-03-30
// =============================================================================
class LogisticToolbar : public QToolBar
{

public:
    //! @name Constructors/Destructor
    //@{
             LogisticToolbar( QMainWindow* pParent, Controllers& controllers );
    virtual ~LogisticToolbar();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LogisticToolbar( const LogisticToolbar& );            //!< Copy constructor
    LogisticToolbar& operator=( const LogisticToolbar& ); //!< Assignement operator
    //@}
};

#endif // __LogisticToolbar_h_
