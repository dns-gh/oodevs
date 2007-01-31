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

#include "tools/Server_ABC.h"
#include "master_asn/Asn.h"

namespace DIN { class DIN_Input; }

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
class Master : public tools::Server_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Master( DIN::DIN_MessageService_ABC& messageService, DIN::DIN_Link& link, Model& model, Profile& profile );
    virtual ~Master();
    //@}

    //! @name Operations
    //@{
    void OnReceive( const ASN1T_MsgsOutMaster& message );
    void OnReceive( unsigned int id, DIN::DIN_Input& message );
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
    Model& model_;
    Profile& profile_;
    //@}
};

}

#endif // __Master_h_
