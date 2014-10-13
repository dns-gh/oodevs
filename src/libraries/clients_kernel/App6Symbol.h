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
    //! @name Types
    //@{
    enum E_LevelFilter
    {
        eLevelFilter_Nothing,
        eLevelFilter_Partial,
        eLevelFilter_Full
    };
    //@}

    //! @name Operations
    //@{
    static void SetKarma( std::string& symbol, const Karma& karma );
    static void FilterPerceptionLevel( std::string& symbol, E_LevelFilter filter );
    static std::string GetBase( const std::string& symbol, Karma& karma );
    static std::string FilterNature( const std::string& nature, E_LevelFilter perception );
    //@}
};

}

#endif // __App6Symbol_h_
