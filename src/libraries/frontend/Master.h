// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Master_h_
#define __Master_h_

#include "master_asn/Asn.h"

namespace tools
{
    class MessageSender_ABC;
}

namespace frontend
{
    class Model;
    class Profile;

// =============================================================================
/** @class  Master
    @brief  Master
*/
// Created: SBO 2007-01-25
// =============================================================================
class Master
{

public:
    //! @name Constructors/Destructor
    //@{
             Master( tools::MessageSender_ABC& messageService, const std::string& endpoint, Model& model, Profile& profile );
    virtual ~Master();
    //@}

    //! @name Operations
    //@{
    void OnReceive( const ASN1T_MsgsOutMaster& message );
    void Send     ( const ASN1T_MsgsInMaster&  message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Master( const Master& );            //!< Copy constructor
    Master& operator=( const Master& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void OnReceiveMsgAuthenticationResponse       ( const ASN1T_MsgAuthenticationResponse&        message );
    void OnReceiveMsgDatasetCreation              ( const ASN1T_MsgDatasetCreation&               message );
    void OnReceiveMsgTerrainCreation              ( const ASN1T_MsgTerrainCreation&               message );
    void OnReceiveMsgPhysicalModelCreation        ( const ASN1T_MsgPhysicalModelCreation&         message );
    void OnReceiveMsgExerciseCreation             ( const ASN1T_MsgExerciseCreation&              message );
    void OnReceiveMsgExerciseUpdate               ( const ASN1T_MsgExerciseUpdate&                message );
    void OnReceiveMsgExerciseDestruction          ( const ASN1T_MsgExerciseDestruction&           message );
    void OnReceiveMsgExerciseCreationRequestAck   ( const ASN1T_MsgExerciseCreationRequestAck&    message );
    void OnReceiveMsgExerciseUpdateRequestAck     ( const ASN1T_MsgExerciseUpdateRequestAck&      message );
    void OnReceiveMsgExerciseDestructionRequestAck( const ASN1T_MsgExerciseDestructionRequestAck& message );
    //@}

private:
    //! @name Member data
    //@{
    tools::MessageSender_ABC& messageService_;
    Model& model_;
    Profile& profile_;
    const std::string endpoint_;
    //@}
};

}

#endif // __Master_h_
