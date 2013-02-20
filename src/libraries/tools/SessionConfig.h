// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SessionConfig_h_
#define __SessionConfig_h_

#include "ExerciseConfig.h"
#include <set>

namespace tools
{
// =============================================================================
/** @class  SessionConfig
    @brief  Session configuration
*/
// Created: AGE 2008-03-13
// =============================================================================
class SessionConfig : public ExerciseConfig
{
public:
    //! @name Constructors/Destructor
    //@{
             SessionConfig( RealFileLoaderObserver_ABC& observer );
    virtual ~SessionConfig();
    //@}

    //! @name Operations
    //@{
    virtual std::string GetSessionDir() const;
    virtual std::string GetSessionFile() const;
    virtual std::string BuildSessionChildFile( const std::string& file ) const;

    virtual std::string GetRecordDirectory() const;
    virtual std::string GetCheckpointsDirectory() const;
    virtual std::string GetCheckpointDirectory( const std::string& checkpoint ) const;
    virtual std::string BuildOnLocalCheckpointChildFile( const std::string& checkpoint, const std::string& file ) const;

    virtual bool HasCheckpoint() const;
    virtual bool CanCreateNoPartyObjects() const;
    virtual bool CanCreateParty( unsigned int id ) const;
    virtual std::string GetCheckpointDirectory() const;

    virtual void Parse( int argc, char** argv );
    //@}

protected:
    //! @name Operations
    //@{
    void ExtractParties( const std::string& subset );
    //@}

    //! @name Member data
    //@{
    bool subset_;
    std::set< unsigned int > subsetParties_;
    bool createNoPartyObjects_;
    //@}

private:
    //! @name Member data
    //@{
    std::string sessionConfigFile_;
    std::string sessionName_;
    bool hasCheckPoint_;
    std::string strCheckPointName_;
    //@}
};

}

#endif // __SessionConfig_h_
