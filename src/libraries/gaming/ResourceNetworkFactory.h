// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ResourceNetworkFactory_h_
#define __ResourceNetworkFactory_h_

namespace kernel
{
    class Controller;
}

namespace MsgsSimToClient
{
    class MsgUrbanAttributes_Infrastructures;
}

class Model;
class ResourceNetwork;

// =============================================================================
/** @class  ResourceNetworkFactory
    @brief  ResourceNetworkFactory
*/
// Created: JSR 2010-08-19
// =============================================================================
class ResourceNetworkFactory
{
public:
    //! @name Constructors/Destructor
    //@{
             ResourceNetworkFactory( kernel::Controller& controller, Model& model );
    virtual ~ResourceNetworkFactory();
    //@}

    //! @name Operations
    //@{
    ResourceNetwork* Create( unsigned int id, const MsgsSimToClient::MsgUrbanAttributes_Infrastructures& msg, bool isUrban );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ResourceNetworkFactory( const ResourceNetworkFactory& );            //!< Copy constructor
    ResourceNetworkFactory& operator=( const ResourceNetworkFactory& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    Model& model_;
    //@}
};

#endif // __ResourceNetworkFactory_h_
