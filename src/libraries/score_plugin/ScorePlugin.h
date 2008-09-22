// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ScorePlugin_h_
#define __ScorePlugin_h_

#include "dispatcher/Plugin_ABC.h"
#include <vector>
#include <boost/shared_ptr.hpp>

namespace xml
{
    class xistream;
}

namespace tools
{
    class ExerciseConfig;
}

class Task;

namespace score
{

// =============================================================================
/** @class  ScorePlugin
    @brief  ScorePlugin
*/
// Created: AGE 2008-08-04
// =============================================================================
class ScorePlugin : public dispatcher::Plugin_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ScorePlugin( dispatcher::ClientPublisher_ABC& clients, const tools::ExerciseConfig& config );
    virtual ~ScorePlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive                  ( const ASN1T_MsgsSimToClient& message );
    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile );
    virtual void NotifyClientLeft         ( dispatcher::ClientPublisher_ABC& client );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ScorePlugin( const ScorePlugin& );            //!< Copy constructor
    ScorePlugin& operator=( const ScorePlugin& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void LoadIndicators( const std::string& functions );
    void LoadIndicator ( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::ClientPublisher_ABC& clients_;
    std::vector< boost::shared_ptr< Task > > tasks_;
    //@}
};

}

#endif // __ScorePlugin_h_
