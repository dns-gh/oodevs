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

#include "dispatcher/Registrable_ABC.h"
#include "protocol/MessengerSenders.h"
#include "tools/Resolver.h"
#include "tools/Path.h"
#include <boost/noncopyable.hpp>
#include <ostream>
#include <list>

namespace dispatcher
{
    class Config;
    class ClientPublisher_ABC;
}

namespace tools
{
    class IdManager;
}

namespace plugins
{
namespace messenger
{
    class Note;

// =============================================================================
/** @class  NotesModel
    @brief  NotesModel
*/
// Created: HBD 2010-01-15
// =============================================================================
class NotesModel : private boost::noncopyable
                 , private tools::Resolver< Note >
                 , public dispatcher::Registrable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             NotesModel( const dispatcher::Config& config, dispatcher::ClientPublisher_ABC& clients, tools::IdManager& idManager, const tools::Path& file );
    virtual ~NotesModel();
    //@}

    //! @name Operations
    //@{
    void SaveNotes();
    void UpdateTime( const std::string& time );
    //@}

    //! @name Requests
    //@{
    void HandleRequest( const sword::MarkerCreationRequest&    message, unsigned int context );
    void HandleRequest( const sword::MarkerDestructionRequest& message );
    void HandleRequest( const sword::MarkerUpdateRequest&      message );
    void SendStateToNewClient( dispatcher::ClientPublisher_ABC& publisher ) const;
    //@}
private:
    //! @name Helpers
    //@{
    unsigned int LoadNotes( const tools::Path& filename, unsigned int skip );
    void HandleRequestDestructSingle( Note* note );
    void HandleRequestDestructCascade( Note* note );
    void ReadNote( const std::string& input, std::vector< unsigned int >& notes );
    void WriteNote( std::ostream& os, const Note& note, int& lineNumber, int parentLine );
    void CreateHeader( std::ostream& os );
    unsigned int CreateNote( std::vector< std::string >& note, const unsigned int parent );
    virtual void RegisterIn( directia::brain::Brain& brain );
    void CreateFromFile( const tools::Path& filename, bool tail );
    void OpenContext( const tools::Path& name );
    void CloseContext();
    void ClearContext();
    //@}

    //! @name Types
    //@{
    typedef std::list< unsigned long > T_List;
    typedef T_List::const_iterator     CIT_List;
    typedef std::map< tools::Path, T_List > T_ContextNotes;
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Config&        config_;
    dispatcher::ClientPublisher_ABC& clients_;
    tools::IdManager&                idManager_;
    const tools::Path                fileName_;
    std::list< unsigned int >        headNotes_;
    std::string                      currentTime_;
    unsigned int                     cursor_;
    T_ContextNotes                   contexts_;
    tools::Path                      currentContext_;
    //@}
};

}
}

#endif // __NotesModel_h_
