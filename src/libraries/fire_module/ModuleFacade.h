// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef fire_module_ModuleFacade_h
#define fire_module_ModuleFacade_h

#include <tools/Map.h>
#include <wrapper/Module.h>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace sword
{
namespace fire
{
    class Knowledge_RapForLocal;

// =============================================================================
/** @class  ModuleFacade
    @brief  Module facade
*/
// Created: MCO 2012-03-19
// =============================================================================
class ModuleFacade : private sword::wrapper::Module
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ModuleFacade( const wrapper::View& model );
    //@}

    //! @name Hooks
    //@{
    double GetForceRatio( const wrapper::View& model, const wrapper::View& entity );
    void GetDangerousEnemies( const wrapper::View& model, const wrapper::View& entity,
                              void(*visitor)( const SWORD_Model* knowledge, void* userData ), void* userData );
    //@}

    //! @name Member data
    //@{
    tools::Map< const SWORD_Model*, std::pair< double, std::size_t > > weapons;
    //@}
};

}
}

#endif // fire_module_ModuleFacade_h
