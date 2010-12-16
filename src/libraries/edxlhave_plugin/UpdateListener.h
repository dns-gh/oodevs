// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __dispatcher_plugins_edxl_UpdateListener_h_
#define __dispatcher_plugins_edxl_UpdateListener_h_

#include "ResponseHandler_ABC.h"

namespace sword
{
    class MissionParameter_Value;
    class MagicActionUpdateObject;
}

namespace kernel
{
    class StaticModel;
}

namespace dispatcher
{
    class Model_ABC;
    class SimulationPublisher_ABC;
}

namespace xml
{
    class xistream;
}

namespace plugins
{
namespace edxl
{

// =============================================================================
/** @class  UpdateListener
    @brief  UpdateListener
*/
// Created: JCR 2010-05-31
// =============================================================================
class UpdateListener : public ResponseHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             UpdateListener( const dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel, dispatcher::SimulationPublisher_ABC& simulation );
    virtual ~UpdateListener();
    //@}

    //! @name Operations
    //@{
    void Handle( xml::xisubstream xis );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UpdateListener( const UpdateListener& );            //!< Copy constructor
    UpdateListener& operator=( const UpdateListener& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadHospitalStatus( xml::xistream& xis );
    void SendHospital( xml::xistream& xis );
    void UpdateFacilityStatus( xml::xistream& xis, sword::MissionParameter_Value& parameters );
    void UpdateCapacityStatus( xml::xistream& xis, sword::MissionParameter_Value& parameters );
    //@}

    //! @name
    //@{
    void Send( sword::MagicActionUpdateObject& asn );
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Model_ABC& model_;
    const kernel::StaticModel& static_;
    dispatcher::SimulationPublisher_ABC& publisher_;
    //@}
};

}
}

#endif // __dispatcher_plugins_edxl_UpdateListener_h_
