// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __SaverFacade_h_
#define __SaverFacade_h_

#include "MessageFilter.h"
#include "MessageHandler_ABC.h"

namespace MsgsSimToClient
{
    class MsgSimToClient;
}

namespace dispatcher
{
    class Config;
    class Model;
    class Saver;

// =============================================================================
/** @class  SaverFacade
    @brief  SaverFacade
*/
// Created: AGE 2007-04-10
// =============================================================================
class SaverFacade : public MessageHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SaverFacade( Model& model, const Config& config );
    virtual ~SaverFacade();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const MsgsSimToClient::MsgSimToClient& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SaverFacade( const SaverFacade& );            //!< Copy constructor
    SaverFacade& operator=( const SaverFacade& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void SaveUpdate( const MsgsSimToClient::MsgSimToClient& message );
    void StartFrame( const MsgsSimToClient::MsgSimToClient& message );
    void EndFrame  ( const MsgsSimToClient::MsgSimToClient& message );
    //@}

private:
    //! @name Member data
    //@{
    Model& model_;
    std::auto_ptr< Saver > saver_;
    std::string encodingBuffer_;
    unsigned frameCount_;
    MessageFilter filter_;
    //@}
};

}

#endif // __SaverFacade_h_
