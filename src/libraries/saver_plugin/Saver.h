// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Saver_h_
#define __Saver_h_

#include "dispatcher/Frames.h"
#include "tools/FileWrapper.h"
#include <tools/Path.h>
#include <boost/noncopyable.hpp>

namespace dispatcher
{
    class ClientPublisher_ABC;
    class Config;
}

namespace sword
{
    class ControlInformation;
    class SimToClient;
}

namespace plugins
{
namespace saver
{
    class Savable_ABC;

// =============================================================================
/** @class  Saver
    @brief  Saver
*/
// Created: AGE 2007-04-10
// =============================================================================
class Saver : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Saver( dispatcher::ClientPublisher_ABC& client, const dispatcher::Config& config );
    virtual ~Saver();
    //@}

    //! @name Operations
    //@{
    void ControlInformation( const sword::ControlInformation& controlInformation );
    void CreateNewFragment ( bool first = false );
    void StartFrame        ( const Savable_ABC& message, const sword::SimToClient& msg );
    void SaveUpdateMessage ( const Savable_ABC& message );
    void EndFrame          ( const Savable_ABC& message );
    void SaveKeyFrame      ( const Savable_ABC& message );
    //@}

private:
    //! @name Helpers
    //@{
    void Flush();
    void CopyFromCurrentToFolder();
    void TerminateFragment();
    void GenerateInfoFile() const;
    void UpdateFragments();
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::ClientPublisher_ABC& client_;
    static const tools::Path currentFolderName_;
    tools::Path recorderDirectory_;
    std::string simTime_;
    std::string realTime_;
    dispatcher::Frame current_;
    tools::Ofstream index_;
    tools::Ofstream keyIndex_;
    tools::Ofstream key_;
    tools::Ofstream update_;
    unsigned int frameCount_;
    unsigned int fragmentFirstFrame_;
    unsigned int currentFolder_;
    bool hasCheckpoint_;
    //@}
};

}
}

#endif // __Saver_h_
