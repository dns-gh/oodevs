// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Loader_h_
#define __Loader_h_

#include <boost/noncopyable.hpp>

namespace sword
{
    class TimeTable;
}

namespace dispatcher
{
    class Config;
    class ClientPublisher_ABC;
    class MessageHandler_ABC;
    class MessageLoader;
    class ReplayModel_ABC;

// =============================================================================
/** @class  Loader
    @brief  Loader
*/
// Created: AGE 2007-04-10
// =============================================================================
class Loader : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Loader( ReplayModel_ABC& model, MessageHandler_ABC& handler, const Config& config, ClientPublisher_ABC& clients );
    virtual ~Loader();
    //@}

    //! @name Operations
    //@{
    bool Start();
    void SkipToFrame( unsigned int count );
    bool Tick();

    unsigned int GetCurrentTick() const;
    unsigned int GetTickNumber() const;
    unsigned int GetFirstTick() const;

    void FillTimeTable( sword::TimeTable& msg, unsigned int beginTick, unsigned int endTick ) const;
    unsigned int FindTickForDate( const std::string& GDHDate ) const;
    const std::string& GetEndDateTime() const;
    void SendTimeskips( ClientPublisher_ABC& client ) const;

    void ReloadAll();
    //@}

private:
    void SkipToFrame( unsigned int keyFrame, unsigned int frame );

private:
    //! @name Member data
    //@{
    ReplayModel_ABC& model_;
    MessageHandler_ABC& handler_;
    std::unique_ptr< MessageLoader > loader_;
    unsigned int keyFrame_;
    unsigned int frame_;
    //@}
};

}

#endif // __Loader_h_
