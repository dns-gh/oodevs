// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ESRI_ConnectorFacade_h_
#define __ESRI_ConnectorFacade_h_

#include "game_asn/Asn.h"

namespace kernel
{
    class AgentTypes;
}

namespace dispatcher
{
    class Model;

namespace esri
{
    class Connector;
    class Config;
    class World;

// =============================================================================
/** @class  ConnectorFacade
    @brief  ConnectorFacade    
*/
// Created: JCR 2007-04-30
// =============================================================================
class ConnectorFacade
{

public:
    //! @name Constructors/Destructor
    //@{
            ConnectorFacade( const Model& model, const esri::Config& config );
    virtual ~ConnectorFacade();
    //@}

    //! @name Operations
    //@{
    void Update();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ConnectorFacade( const ConnectorFacade& );            //!< Copy constructor
    ConnectorFacade& operator=( const ConnectorFacade& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    const Model&                        model_;
    std::auto_ptr< World >              world_;
    std::auto_ptr< kernel::AgentTypes > types_;
    std::auto_ptr< Connector >          connector_;
    //@}
};

}
}

#endif // __ConnectorFacade_h_
