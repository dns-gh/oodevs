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
             SessionConfig( const boost::shared_ptr< RealFileLoaderObserver_ABC>& observer );
    virtual ~SessionConfig();
    //@}

    //! @name Operations
    //@{
    virtual Path GetSessionDir() const;
    virtual Path GetSessionFile() const;
    virtual Path BuildSessionChildFile( const Path& file ) const;

    virtual Path GetRecordDirectory() const;
    virtual Path GetCheckpointsDirectory() const;
    virtual Path GetCheckpointDirectory() const;
    virtual Path GetCheckpointDirectory( const Path& checkpoint ) const;
    virtual Path BuildOnLocalCheckpointChildFile( const Path& checkpoint, const Path& file ) const;

    virtual bool HasCheckpoint() const;
    virtual bool CanCreateNoPartyObjects() const;
    virtual bool CanCreateParty( unsigned int id ) const;

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
    Path sessionConfigFile_;
    Path sessionName_;
    Path strCheckPointName_;
    bool hasCheckPoint_;
    //@}
};

}

#endif // __SessionConfig_h_
