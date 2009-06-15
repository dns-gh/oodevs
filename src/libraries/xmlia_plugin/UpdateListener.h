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

#include "ResponseHandler_ABC.h"

namespace dispatcher
{
    class Model;
    class SimulationPublisher_ABC;
}

namespace xml
{
    class xistream;
}

namespace plugins
{
namespace xmlia
{
    class Publisher_ABC;
    class ResponseHandler_ABC;

// =============================================================================
/** @class  UpdateListener
    @brief  UpdateListener
    // $$$$ AGE 2008-05-30: rename
*/
// Created: SBO 2008-05-16
// =============================================================================
class UpdateListener : private ResponseHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             UpdateListener( Publisher_ABC& publisher, const dispatcher::Model& model, dispatcher::SimulationPublisher_ABC& simulation );
    virtual ~UpdateListener();
    //@}

    //! @name Operations
    //@{
    void PullOrders();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UpdateListener( const UpdateListener& );            //!< Copy constructor
    UpdateListener& operator=( const UpdateListener& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Handle( const std::string& response );
    void PullOrders( const std::string& time );
    void FindLastDate( const std::string& response );
    void ReadOrder( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    Publisher_ABC& publisher_;
    std::auto_ptr< ResponseHandler_ABC > orderProcessor_;
    std::string lastUpdateTime_;
    bool receivedAnswer_;
    //@}
};

}
}

#endif // __UpdateListener_h_
