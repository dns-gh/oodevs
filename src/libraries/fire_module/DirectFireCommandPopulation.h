// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef fire_module_DirectFireCommandPopulation_h
#define fire_module_DirectFireCommandPopulation_h

#include "RoleAction_DirectFiring.h"
#include <boost/noncopyable.hpp>

namespace sword
{
namespace wrapper
{
    class View;
}

namespace fire
{
    class ModuleFacade;

// =============================================================================
/** @class  DirectFireCommandPopulation
    @brief  Direct fire on population command
*/
// Created: MCO 2012-09-17
// =============================================================================
class DirectFireCommandPopulation : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
     DirectFireCommandPopulation( ModuleFacade& module, const wrapper::View& parameters, const wrapper::View& model, std::size_t identifier );
    ~DirectFireCommandPopulation();
    //@}

    //! @name Operations
    //@{
    void Execute( const wrapper::View& parameters, const wrapper::View& model ) const;
    void ExecutePaused( const wrapper::View& parameters, const wrapper::View& model ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void PostCallback( int code ) const;
    //@}

private:
    //! @name Member data
    //@{
    const size_t commandIdentifier_;
    const unsigned int identifier_;
    const RoleAction_DirectFiring role_;
    //@}
};

}
}

#endif // fire_module_DirectFireCommandPopulation_h
