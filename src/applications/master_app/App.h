// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __App_h_
#define __App_h_

#include <string>

namespace master
{
    class Master;
}

// =============================================================================
/** @class  App
    @brief  App
*/
// Created: NLD 2006-10-10
// =============================================================================
class App
{

public:
    //! @name Constructors/Destructor
    //@{
     App( int argc, char** argv );
    ~App();
    //@}

    //! @name Main
    //@{
    int Execute(); 
    //@}

private:
    //! @name Copy/Assignement
    //@{
    App( const App& );            //!< Copy constructor
    App& operator=( const App& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    master::Master* pMaster_;
    //@}
};

#endif // __App_h_
