// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __plugins_timeline_Task_h_
#define __plugins_timeline_Task_h_

namespace xml
{
    class xibufferstream;
    class xistream;
}

namespace plugins
{
namespace timeline
{
    class ActionPublisher;

// =============================================================================
/** @class  Task
    @brief  Task
*/
// Created: JCR 2010-09-08
// =============================================================================
class Task
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Task( xml::xistream& xis );
    virtual ~Task();
    //@}

    //! @name Operations
    //@{
    void Execute( ActionPublisher& publisher );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Task( const Task& );            //!< Copy constructor
    Task& operator=( const Task& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    bool triggered_;
    std::unique_ptr< xml::xistream > xis_;
    //@}
};

}
}

#endif // __Task_h_
