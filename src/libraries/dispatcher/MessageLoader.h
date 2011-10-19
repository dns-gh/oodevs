// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MessageLoader_h_
#define __MessageLoader_h_

#include "Frames.h"
#include "MessageLoader_ABC.h"
#include <tools/thread/ThreadPool.h>
#include <boost/shared_ptr.hpp>
#include <boost/filesystem/path.hpp>
#include <vector>
#include <fstream>

namespace sword
{
    class TimeTable;
}

namespace dispatcher
{
    class Config;
    class MessageHandler_ABC;
    struct Buffer;

// =============================================================================
/** @class  MessageLoader
    @brief  Message loader
*/
// Created: AGE 2007-07-09
// =============================================================================
class MessageLoader : public MessageLoader_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MessageLoader( const Config& config, bool threaded );
    virtual ~MessageLoader();
    //@}

    //! @name Operations
    //@{
    virtual bool LoadFrame   ( unsigned int frameNumber, MessageHandler_ABC& handler, const T_Callback& callback = T_Callback() );
    virtual void LoadKeyFrame( unsigned int frameNumber, MessageHandler_ABC& handler, const T_Callback& callback = T_Callback() );
    virtual unsigned int GetTickNumber() const;
    unsigned int FindKeyFrame( unsigned int frameNumber );
    void FillTimeTable( sword::TimeTable& msg, unsigned int beginTick, unsigned int endTick ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< Frame > T_Frames;
    typedef std::vector< KeyFrame > T_KeyFrames;
    typedef T_KeyFrames::const_iterator CIT_KeyFrames;
    typedef std::map< std::string, std::pair< unsigned int, unsigned int > > T_FragmentsInfos;
    typedef T_FragmentsInfos::const_iterator CIT_FragmentsInfos;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MessageLoader( const MessageLoader& );            //!< Copy constructor
    MessageLoader& operator=( const MessageLoader& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ScanData();
    void AddFolder( const std::string& folderName );
    bool OpenFile( std::ifstream& stream, const std::string& folder, const std::string& file ) const;
    bool SwitchToFragment( unsigned int& frameNumber );
    void Load( std::ifstream& in, unsigned from, unsigned size, MessageHandler_ABC& handler, const T_Callback& callback );
    void LoadFrameInThread( const std::string& folder, unsigned int frameNumber, MessageHandler_ABC& handler, const T_Callback& callback );
    void LoadKeyFrameInThread( const std::string& folder, unsigned int frameNumber, MessageHandler_ABC& handler, const T_Callback& callback );
    void LoadBuffer( const boost::shared_ptr< Buffer >& buffer, MessageHandler_ABC& handler, const T_Callback& callback );
    void LoadSimToClientMessage( char*& input, MessageHandler_ABC& handler );
    //@}

private:
    //! @name Static member data
    //@{
    static const std::string infoFileName_;
    static const std::string indexFileName_;
    static const std::string keyIndexFileName_;
    static const std::string keyFileName_;
    static const std::string updateFileName_;
    static boost::mutex filesAccessMutex_;
    //@}

private:
    //! @name Member data
    //@{
    const Config& config_;
    unsigned int firstTick_;
    unsigned int tickCount_;
    bool initReady_;
    mutable boost::mutex dataAccessMutex_;
    boost::mutex initMutex_;
    boost::condition_variable initCondition_;
    std::auto_ptr< boost::thread > folderObserver_;
    std::auto_ptr< tools::thread::ThreadPool > disk_;
    std::auto_ptr< tools::thread::ThreadPool > cpu_;
    T_FragmentsInfos fragmentsInfos_;
    std::string currentOpenFolder_;
    std::ifstream index_;
    std::ifstream keyIndex_;
    std::ifstream updates_;
    std::ifstream keys_;
    T_Frames frames_;
    T_KeyFrames keyFrames_;
    bool synchronisation_;
    //@}
};

}

#endif // __MessageLoader_h_
