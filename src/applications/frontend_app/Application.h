// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Application_h_
#define __Application_h_

#include <qapplication.h>

// =============================================================================
/** @class  Application
    @brief  Application
*/
// Created: SBO 2007-01-26
// =============================================================================
class Application : public QApplication
{
public:
    //! @name Constructors/Destructor
    //@{
             Application( int argc, char** argv, const QString& locale );
    virtual ~Application();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Application( const Application& );            //!< Copy constructor
    Application& operator=( const Application& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void AddTranslator( const char* t, const QString& locale );
    //@}
};

#endif // __Application_h_
