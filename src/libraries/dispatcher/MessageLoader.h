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

#include "game_asn/Asn.h"
#include "Frames.h"
#include <vector>
#include <fstream>

namespace tools
{
    class InputBinaryWrapper;
}

namespace dispatcher
{
    class Config;
    class MessageHandler_ABC;

// =============================================================================
/** @class  MessageLoader
    @brief  Message loader
*/
// Created: AGE 2007-07-09
// =============================================================================
class MessageLoader
{

public:
    //! @name Constructors/Destructor
    //@{
             MessageLoader( const Config& config, const std::string& records );
    virtual ~MessageLoader();
    //@}

    //! @name Operations
    //@{
    bool         LoadFrame   ( unsigned int frameNumber, MessageHandler_ABC& handler );
    unsigned int LoadKeyFrame( unsigned int frameNumber, MessageHandler_ABC& handler );

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
    void LoadIndex( const std::string& file );
    void LoadKeyIndex( const std::string& file );
    void LoadSimToClientMessage( std::ifstream& input, unsigned count, MessageHandler_ABC& handler );
    void LoadSimToClientMessage( tools::InputBinaryWrapper& input, MessageHandler_ABC& handler );
    void LoadKeyFrame( unsigned frame );
    //@}

private:
    //! @name Member data
    //@{
    T_Frames frames_;
    T_KeyFrames keyFrames_;
    std::ifstream updates_;
    std::ifstream keys_;
    ASN1OCTET buffer_[100000];
    //@}
};

}

#endif // __MessageLoader_h_
