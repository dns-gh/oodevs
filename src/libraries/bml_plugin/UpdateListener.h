// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UpdateListener_h_
#define __UpdateListener_h_

#include "game_asn/Simulation.h"

namespace dispatcher
{
    class Model;
    class SimulationPublisher_ABC;
}

namespace bml
{
    class Publisher;
    class ResponseHandler_ABC;

// =============================================================================
/** @class  UpdateListener
    @brief  UpdateListener
*/
// Created: SBO 2008-05-16
// =============================================================================
class UpdateListener
{

public:
    //! @name Constructors/Destructor
    //@{
             UpdateListener( Publisher& publisher, const dispatcher::Model& model, dispatcher::SimulationPublisher_ABC& simulation );
    virtual ~UpdateListener();
    //@}

    //! @name Operations
    //@{
    void Update( const ASN1T_MsgControlBeginTick& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UpdateListener( const UpdateListener& );            //!< Copy constructor
    UpdateListener& operator=( const UpdateListener& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void PullOrders( const std::string& time );
    //@}

private:
    //! @name Member data
    //@{
    Publisher& publisher_;
    std::auto_ptr< ResponseHandler_ABC > orderProcessor_;
    std::string lastUpdateTime_;
    //@}
};

}

#endif // __UpdateListener_h_
