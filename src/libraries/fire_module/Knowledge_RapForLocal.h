// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef fire_module_Knowledge_RapForLocal_h
#define fire_module_Knowledge_RapForLocal_h

#include "Knowledge_RapFor_ABC.h"
#include <wrapper/View.h>
#include <vector>

namespace sword
{
namespace fire
{
// =============================================================================
/** @class  Knowledge_RapForLocal
    @brief  Knowledge local RapFor
*/
// Created: NLD 2004-04-07
// =============================================================================
class Knowledge_RapForLocal : public Knowledge_RapFor_ABC
{
public:
    //! @name Types
    //@{
    typedef std::vector< const SWORD_Model* >   T_KnowledgeAgents;
    typedef T_KnowledgeAgents::const_iterator CIT_KnowledgeAgents;
    //@}

    //! @name Accessors
    //@{
    const T_KnowledgeAgents& GetDangerousEnemies( const wrapper::View& model, const wrapper::View& entity );
    //@}

private:
    //! @name Operations
    //@{
    virtual void Update( const wrapper::View& model, const wrapper::View& entity );
    //@}

private:
    //! @name Member data
    //@{
    T_KnowledgeAgents dangerousEnemies_;
    //@}
};

}
}

#endif // fire_module_Knowledge_RapForLocal_h
