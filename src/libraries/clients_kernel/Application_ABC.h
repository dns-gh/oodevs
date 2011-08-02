// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Application_ABC_h_
#define __Application_ABC_h_

#pragma warning( push, 0 )
#include <QtGui/qapplication.h>
#pragma warning( pop )

// =============================================================================
/** @class  Application_ABC
    @brief  Application_ABC
*/
// Created: HBD 2010-06-28
// =============================================================================
class Application_ABC : public QApplication
{
public:
    //! @name Constructors/Destructor
    //@{
             Application_ABC( int& argc, char** argv ) ;
    virtual ~Application_ABC();
    //@}

    //! @name Operations
    void AddTranslator( const QString& locale, const char* t );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Application_ABC( const Application_ABC& );            //!< Copy constructor
    Application_ABC& operator=( const Application_ABC& ); //!< Assignment operator
    //@}
};

#endif // __Application_ABC_h_
