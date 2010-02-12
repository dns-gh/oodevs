// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __NotesModel_h_
#define __NotesModel_h_

#include "game_asn/Messenger.h"
#include "tools/Resolver.h"

namespace dispatcher
{
    class Config;
    class ClientPublisher_ABC;
}

namespace plugins
{
    namespace messenger 
    {
        class IdManager;
        class Note;

// =============================================================================
/** @class  NotesModel
    @brief  NotesModel
*/
// Created: HBD 2010-01-15
// =============================================================================
class NotesModel : public tools::Resolver< Note >
{

public:
    //! @name Constructors/Destructor
    //@{
             NotesModel( const dispatcher::Config& config, dispatcher::ClientPublisher_ABC& clients, IdManager& idManager );
    virtual ~NotesModel();
    //@}

    //! @name Operations
    //@{
    void Save( const std::string& directory ) const;
    //@}

    //! @name Requests
    //@{
    void HandleRequest( const ASN1T_MsgNoteCreationRequest&    message );
    void HandleRequest( const ASN1T_MsgNoteDestructionRequest& message );
    void HandleRequest( const ASN1T_MsgNoteUpdateRequest&      message );
    void SendStateToNewClient( dispatcher::ClientPublisher_ABC& publisher ) const;

    void Publish( const Note& note );

    //@}
private:
    //! @name Copy/Assignment
    //@{
    NotesModel( const NotesModel& );            //!< Copy constructor
    NotesModel& operator=( const NotesModel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void HandleRequestDestructSingle( Note* note );
    void HandleRequestDestructCascade( Note* note );
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Config& config_;
    dispatcher::ClientPublisher_ABC& clients_;
    IdManager& idManager_;
    //@}
};

}

}
#endif // __NotesModel_h_
