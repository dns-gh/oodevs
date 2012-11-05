// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DEC_TelepathyFunctions_h_
#define __DEC_TelepathyFunctions_h_

#include <boost/shared_ptr.hpp>

namespace sword
{
    class Brain;
}

class DEC_Decision_ABC;
class DEC_Knowledge_Agent;
class DEC_Knowledge_Object;

// =============================================================================
/** @class  DEC_TelepathyFunctions
    @brief  DEC_TelepathyFunctions
*/
// Created: LDC 2009-07-31
// =============================================================================
class DEC_TelepathyFunctions
{
public:
    //! @name Registering all those functions in the brain
    //@{
    static void Register( sword::Brain& brain );
    //@}

    //! @name Functions used to directly access or modify another brain
    //@{
    static void CopyPlotsRavitaillement( DEC_Decision_ABC* pPion, const std::vector< boost::shared_ptr< DEC_Knowledge_Object > >& value );
    static std::vector< boost::shared_ptr< DEC_Knowledge_Agent > > GetListeEnisTirAutorise( DEC_Decision_ABC* pPion );
    static void RemoveAgentFromListeEnisTirAutorise( DEC_Decision_ABC* pPion, boost::shared_ptr< DEC_Knowledge_Agent > eni );
    //@}
};

#endif // __DEC_TelepathyFunctions_h_
