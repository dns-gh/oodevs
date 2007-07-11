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

namespace tools
{
    class InputBinaryWrapper;
}

namespace dispatcher
{
    class Config;
    class ReplayModel_ABC;
    class MessageHandler_ABC;
    class MessageLoader;

// =============================================================================
/** @class  Loader
    @brief  Loader
*/
// Created: AGE 2007-04-10
// =============================================================================
class Loader
{

public:
    //! @name Constructors/Destructor
    //@{
             Loader( ReplayModel_ABC& model, MessageHandler_ABC& handler, const Config& config, const std::string& records );
    virtual ~Loader();
    //@}

    //! @name Operations
    //@{
    void SkipToFrame( unsigned count ); 
    bool Tick();

    unsigned GetCurrentTick() const;
    unsigned GetTickNumber() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Loader( const Loader& );            //!< Copy constructor
    Loader& operator=( const Loader& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    bool RequiresKeyFrame( unsigned frame );
    //@}

private:
    //! @name Member data
    //@{
    ReplayModel_ABC&               model_;
    MessageHandler_ABC&            handler_;
    std::auto_ptr< MessageLoader > loader_;
    unsigned currentFrame_;
    //@}
};

}

#endif // __Loader_h_
