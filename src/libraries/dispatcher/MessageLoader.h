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

#include "game_asn/Simulation.h"
#include "Frames.h"
#include "MessageLoader_ABC.h"
#include "tools/thread/ThreadPool.h"
#include <vector>
#include <fstream>
#include <boost/shared_ptr.hpp>

namespace tools
{
    class InputBinaryWrapper;
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
    virtual bool         LoadFrame   ( unsigned int frameNumber, MessageHandler_ABC& handler, const T_Callback& callback = T_Callback() );
    virtual unsigned int LoadKeyFrame( unsigned int frameNumber, MessageHandler_ABC& handler, const T_Callback& callback = T_Callback()  );
    void Synchronize();
    void ReloadIndices();
    unsigned int GetTickNumber() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MessageLoader( const MessageLoader& );            //!< Copy constructor
    MessageLoader& operator=( const MessageLoader& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< Frame >    T_Frames;
    typedef std::vector< KeyFrame > T_KeyFrames;
    //@}

    //! @name Helpers
    //@{
    void LoadIndices();
    void Load( std::ifstream& in, unsigned from, unsigned size, MessageHandler_ABC& handler, const T_Callback& callback );
    void LoadBuffer( const boost::shared_ptr< Buffer >& buffer, MessageHandler_ABC& handler, const T_Callback& callback );
    void LoadSimToClientMessage( unsigned char*& input, MessageHandler_ABC& handler );
    //@}

private:
    //! @name Member data
    //@{
    std::ifstream index_;
    std::ifstream keyIndex_;
    std::ifstream updates_;
    std::ifstream keys_;

    T_Frames frames_;
    T_KeyFrames keyFrames_;

    std::auto_ptr< tools::thread::ThreadPool > disk_;
    std::auto_ptr< tools::thread::ThreadPool > cpu_;
    //@}
};

}

#endif // __MessageLoader_h_
