// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __edxl_UpdateListener_h_
#define __edxl_UpdateListener_h_

#include "ResponseHandler_ABC.h"

namespace sword
{
    class MissionParameter_Value;
    class MagicActionUpdateObject;
}

namespace xml
{
    class xistream;
}
    
class SwordClient;

namespace edxl
{

    class Model;

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
             UpdateListener( const Model& model, SwordClient& client );
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
    const Model& model_;
    SwordClient& client_;
    //@}
};

}

#endif // __edxl_UpdateListener_h_
