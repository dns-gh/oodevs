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

#include "protocol/MessengerSenders.h"
#include "tools/Resolver.h"
#include <fstream>
#include <list>

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
             NotesModel( const dispatcher::Config& config, dispatcher::ClientPublisher_ABC& clients, IdManager& idManager , const std::string& file);
    virtual ~NotesModel();
    //@}

    //! @name Operations
    //@{
    void Save( const std::string& directory ) const;
    //@}

    //! @name Requests
    //@{
    void HandleRequest( const MsgsClientToMessenger::MsgMarkerCreationRequest&    message );
    void HandleRequest( const MsgsClientToMessenger::MsgMarkerDestructionRequest& message );
    void HandleRequest( const MsgsClientToMessenger::MsgMarkerUpdateRequest&      message );
    void SendStateToNewClient( dispatcher::ClientPublisher_ABC& publisher ) const;

    void Publish( const Note& note );

    void LoadNotes( const std::string filename );
    void SaveNotes();

    void UpdateTime( std::string time );
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
    void WriteNote( const Note& note, int& lineNumber, int parentLine );
    void ReadNote( const std::string& input, std::vector< unsigned int >& notes );
    void CreateHeader();
    unsigned int CreateNote( std::vector<std::string>& note, const unsigned int parent );
    //@}

    //! @name Types
    //@{
    typedef std::list< unsigned long > T_List;
    typedef T_List::const_iterator     CIT_List;
    //@}

private:
    //! @name Member data
    //@{
    static const unsigned int        headerLines_ = 6;
    const dispatcher::Config&        config_;
    dispatcher::ClientPublisher_ABC& clients_;
    IdManager&                       idManager_;
    std::list<unsigned int>          headNotes_;
    std::ofstream                    file_;
    std::string                      fileName_;
    std::string                      currentTime_;
    //@}
};

}

}
#endif // __NotesModel_h_
