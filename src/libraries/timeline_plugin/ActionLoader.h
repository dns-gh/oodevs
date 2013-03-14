// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __plugins_timeline_ActionLoader_h_
#define __plugins_timeline_ActionLoader_h_

namespace xml
{
    class xistream;
}

namespace dispatcher
{
    class Logger_ABC;
}

namespace tools
{
    class Path;
}

namespace plugins
{
namespace timeline
{
    class Publisher_ABC;

// =============================================================================
/** @class  ActionLoader
    @brief  ActionLoader
*/
// Created: JCR 2010-09-08
// =============================================================================
class ActionLoader
{
public:
    //! @name Constructors/Destructor
    //@{
             ActionLoader( long scenarioId, long actorId, Publisher_ABC& publisher, dispatcher::Logger_ABC& logger );
    virtual ~ActionLoader();
    //@}

    //! @name Operations
    //@{
    void Load( const tools::Path& filename );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionLoader( const ActionLoader& );            //!< Copy constructor
    ActionLoader& operator=( const ActionLoader& ); //!< Assignment operator
    //@}

    //! @name Helper
    //@{
    void ReadAction( xml::xistream& xis, std::string& error );
    //@}

private:
    //! @name Member data
    //@{
    Publisher_ABC& publisher_;
    long scenarioId_;
    long actorId_;
	dispatcher::Logger_ABC* logger_;
    //@}
};

}
}

#endif // __plugins_timeline_ActionLoader_h_
