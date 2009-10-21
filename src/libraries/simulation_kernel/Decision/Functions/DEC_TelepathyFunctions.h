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

namespace directia
{
    class Brain;
}
class DEC_Decision_ABC;
class DEC_Knowledge_Agent;

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
    static void Register( directia::Brain& brain );
    //@}

    //! @name Functions used to directly access or modify another brain
    //@{
    void CopyPlotsRavitaillement( DEC_Decision_ABC* pPion, const std::vector< int >& value );
    std::vector< boost::shared_ptr< DEC_Knowledge_Agent > > GetListeEnisTirAutorise( DEC_Decision_ABC* pPion );
    void RemoveAgentFromListeEnisTirAutorise( DEC_Decision_ABC* pPion, boost::shared_ptr< DEC_Knowledge_Agent > eni );
    //@}
};

#endif // __DEC_TelepathyFunctions_h_
