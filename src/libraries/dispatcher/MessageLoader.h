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
#include <tools/Path.h>
#include "tools/FileWrapper.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <vector>

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
    class WaitEvent;
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
             MessageLoader( const tools::Path& records, bool threaded, ClientPublisher_ABC* clients = 0 );
    virtual ~MessageLoader();
    //@}

    //! @name Operations
    //@{
    virtual bool LoadFrame   ( unsigned int frameNumber, MessageHandler_ABC& handler, const T_Callback& callback = T_Callback() );
    virtual void LoadKeyFrame( unsigned int frameNumber, MessageHandler_ABC& handler, const T_Callback& callback = T_Callback() );
    virtual unsigned int GetTickNumber() const;
    virtual unsigned int GetFirstTick() const;
    unsigned int FindKeyFrame( unsigned int frameNumber );
    unsigned int FindTickForDate( const std::string& GDHDate ) const;
    const std::string& GetEndDateTime() const;
    void FillTimeTable( sword::TimeTable& msg, unsigned int beginTick, unsigned int endTick ) const;
    void ReloadAllFragmentsInfos();
    //@}

public:
    //! @name Types
    //@{
    typedef std::vector< Frame > T_Frames;
    typedef std::vector< KeyFrame > T_KeyFrames;
    typedef T_KeyFrames::const_iterator CIT_KeyFrames;
    typedef std::map< tools::Path, std::pair< unsigned int, unsigned int > > T_FragmentsInfos;
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
    bool ScanFolder( const tools::Path& path, bool forceAdd );
    void ScanDataFolders( bool );
    void AddFolder( const tools::Path& folderName );
    bool SwitchToFragment( unsigned int& frameNumber );
    void Load( tools::Ifstream& in, unsigned from, unsigned size, MessageHandler_ABC& handler, const T_Callback& callback, const tools::Path& filename );
    void LoadFrameInThread( const tools::Path& folder, unsigned int frameNumber, MessageHandler_ABC& handler, const T_Callback& callback );
    void LoadKeyFrameInThread( const tools::Path& folder, unsigned int frameNumber, MessageHandler_ABC& handler, const T_Callback& callback );
    void LoadBuffer( const boost::shared_ptr< Buffer >& buffer, MessageHandler_ABC& handler, const T_Callback& callback );
    size_t LoadSimToClientMessage( const char* input, size_t size, MessageHandler_ABC& handler );
    //@}

private:
    //! @name Member data
    //@{
    const tools::Path records_;
    ClientPublisher_ABC* clients_;
    unsigned int firstTick_;
    unsigned int tickCount_;
    std::string endDateTime_;
    mutable boost::mutex access_;
    std::unique_ptr< tools::WaitEvent > init_;
    std::unique_ptr< tools::WaitEvent > quit_;
    std::unique_ptr< boost::thread > folderObserver_;
    std::unique_ptr< tools::ThreadPool > disk_;
    std::unique_ptr< tools::ThreadPool > cpu_;
    T_FragmentsInfos fragmentsInfos_;
    tools::Path currentOpenFolder_;
    tools::Ifstream updates_;
    tools::Ifstream keys_;
    T_Frames frames_;
    T_KeyFrames keyFrames_;
    //@}
};
}

#endif // __MessageLoader_h_
