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
#include <boost/shared_ptr.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/thread/mutex.hpp>
#include <vector>
#include <fstream>

namespace boost
{
    class thread;
}

namespace sword
{
    class TimeTable;
}

namespace tools
{
    class ThreadPool;
    struct WaitEvent;
}

namespace dispatcher
{
    class ClientPublisher_ABC;
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
             MessageLoader( const boost::filesystem::path& records, bool threaded, ClientPublisher_ABC* clients = 0 );
    virtual ~MessageLoader();
    //@}

    //! @name Operations
    //@{
    virtual bool LoadFrame   ( unsigned int frameNumber, MessageHandler_ABC& handler, const T_Callback& callback = T_Callback() );
    virtual void LoadKeyFrame( unsigned int frameNumber, MessageHandler_ABC& handler, const T_Callback& callback = T_Callback() );
    virtual unsigned int GetTickNumber() const;
    virtual unsigned int GetFirstTick() const;
    unsigned int FindKeyFrame( unsigned int frameNumber );
    void FillTimeTable( sword::TimeTable& msg, unsigned int beginTick, unsigned int endTick ) const;
    void ReloadAllFragmentsInfos();
    //@}

public:
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
    void ScanDataFolders( bool );
    void AddFolder( const std::string& folderName );
    bool SwitchToFragment( unsigned int& frameNumber );
    void Load( std::ifstream& in, unsigned from, unsigned size, MessageHandler_ABC& handler, const T_Callback& callback, const boost::filesystem::path& filename );
    void LoadFrameInThread( const std::string& folder, unsigned int frameNumber, MessageHandler_ABC& handler, const T_Callback& callback );
    void LoadKeyFrameInThread( const std::string& folder, unsigned int frameNumber, MessageHandler_ABC& handler, const T_Callback& callback );
    void LoadBuffer( const boost::shared_ptr< Buffer >& buffer, MessageHandler_ABC& handler, const T_Callback& callback, bool synchronized );
    size_t LoadSimToClientMessage( const char* input, size_t size, MessageHandler_ABC& handler, bool synchronized );
    //@}

private:
    //! @name Member data
    //@{
    const boost::filesystem::path records_;
    ClientPublisher_ABC* clients_;
    unsigned int firstTick_;
    unsigned int tickCount_;
    mutable boost::mutex access_;
    std::auto_ptr< tools::WaitEvent > init_;
    std::auto_ptr< tools::WaitEvent > quit_;
    std::auto_ptr< boost::thread > folderObserver_;
    std::auto_ptr< tools::ThreadPool > disk_;
    std::auto_ptr< tools::ThreadPool > cpu_;
    T_FragmentsInfos fragmentsInfos_;
    std::string currentOpenFolder_;
    std::ifstream updates_;
    std::ifstream keys_;
    T_Frames frames_;
    T_KeyFrames keyFrames_;
    bool synchronisation_;
    //@}
};
}

#endif // __MessageLoader_h_
