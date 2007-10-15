// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __App6Symbol_h_
#define __App6Symbol_h_

#include "Types.h"

namespace kernel
{
    class Karma;

// =============================================================================
/** @class  App6Symbol
    @brief  App6Symbol
*/
// Created: SBO 2007-02-26
// =============================================================================
class App6Symbol
{

public:
    //! @name Constructors/Destructor
    //@{
             App6Symbol();
    virtual ~App6Symbol();
    //@}

    //! @name Operations
    //@{
    static void Initialize();
    static void SetKarma( std::string& symbol, const Karma& karma );
    static void FilterPerceptionLevel( std::string& symbol, E_PerceptionResult perception );
    static void Merge( const std::string& from, std::string& to );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    App6Symbol( const App6Symbol& );            //!< Copy constructor
    App6Symbol& operator=( const App6Symbol& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::map< Karma, char > T_KarmaChars;
    //@}

private:
    //! @name Member data
    //@{
    static T_KarmaChars karmaChars_;
    //@}
};

}

#endif // __App6Symbol_h_
