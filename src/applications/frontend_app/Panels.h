// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Panels_h_
#define __Panels_h_

#include <qvbox.h>

// =============================================================================
/** @class  Panels
    @brief  Panels
*/
// Created: SBO 2007-01-26
// =============================================================================
class Panels : public QVBox
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Panels( QWidget* parent );
    virtual ~Panels();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Panels( const Panels& );            //!< Copy constructor
    Panels& operator=( const Panels& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __Panels_h_
