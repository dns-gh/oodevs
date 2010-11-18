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
#include <fstream>

namespace dispatcher
{
    class Config;
}

namespace MsgsSimToClient
{
    class MsgControlInformation;
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
class Saver
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Saver( const dispatcher::Config& config );
    virtual ~Saver();
    //@}

    //! @name Operations
    //@{
    void ControlInformation( const MsgsSimToClient::MsgControlInformation& controlInformation );
    void CreateNewFragment ( bool first = false );
    void StartFrame        ( const Savable_ABC& message );
    void SaveUpdateMessage ( const Savable_ABC& message );
    void EndFrame          ( const Savable_ABC& message );
    void SaveKeyFrame      ( const Savable_ABC& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Saver( const Saver& );            //!< Copy constructor
    Saver& operator=( const Saver& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Flush();
    void TerminateFragment();
    void GenerateInfoFile() const;
    void UpdateFragments();
    //@}

private:
    //! @name Member data
    //@{
    static const std::string currentFolderName_;
    std::string recorderDirectory_;
    dispatcher::Frame current_;
    std::ofstream index_;
    std::ofstream keyIndex_;
    std::ofstream key_;
    std::ofstream update_;
    unsigned int frameCount_;
    unsigned int fragmentFirstFrame_;
    unsigned int currentFolder_;
    bool hasCheckpoint_;
    //@}
};

}
}

#endif // __Saver_h_
