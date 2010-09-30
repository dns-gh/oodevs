// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __script_plugin_ModelFacade_h_
#define __script_plugin_ModelFacade_h_

#include "dispatcher/Registrable_ABC.h"

namespace dispatcher
{
    class Model_ABC;
}

namespace plugins
{
namespace script
{
    class AgentManipulator;

// =============================================================================
/** @class  ModelFacade
    @brief  ModelFacade
*/
// Created: SBO 2010-09-17
// =============================================================================
class ModelFacade : public dispatcher::Registrable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ModelFacade( const dispatcher::Model_ABC& model );
    virtual ~ModelFacade();
    //@}

    //! @name Operations
    //@{
    virtual void RegisterIn( directia::brain::Brain& brain );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ModelFacade( const ModelFacade& );            //!< Copy constructor
    ModelFacade& operator=( const ModelFacade& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    const AgentManipulator* FindAgent( unsigned long id );
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Model_ABC& model_;
    //@}
};

}
}

#endif // __script_plugin_ModelFacade_h_
