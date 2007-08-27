// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SaverPlugin_h_
#define __SaverPlugin_h_

#include "Plugin_ABC.h"

namespace dispatcher
{
    class Model;
    class Config;
    class SaverFacade;

// =============================================================================
/** @class  SaverPlugin
    @brief  SaverPlugin
*/
// Created: AGE 2007-08-24
// =============================================================================
class SaverPlugin : public Plugin_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             SaverPlugin( Model& model, const Config& config );
    virtual ~SaverPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const ASN1T_MsgsSimToClient& message );
    virtual void NotifyClientAuthenticated( ClientPublisher_ABC& client, Profile_ABC& profile );
    virtual void NotifyClientLeft         ( ClientPublisher_ABC& client );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SaverPlugin( const SaverPlugin& );            //!< Copy constructor
    SaverPlugin& operator=( const SaverPlugin& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< SaverFacade > saver_;
    //@}
};

}

#endif // __SaverPlugin_h_
