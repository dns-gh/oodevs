// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __DetectionReport_h_
#define __DetectionReport_h_

namespace xml
{
    class xostream;
}

namespace dispatcher
{
    class Agent_ABC;
}

namespace plugins
{
namespace bml
{
    class Publisher_ABC;

// =============================================================================
/** @class  DetectionReport
    @brief  DetectionReport
*/
// Created: SBO 2008-07-22
// =============================================================================
class DetectionReport
{
public:
    //! @name Constructors/Destructor
    //@{
             DetectionReport( const dispatcher::Agent_ABC& entity, const dispatcher::Agent_ABC& detected, int level );
    virtual ~DetectionReport();
    //@}

    //! @name Operations
    //@{
    void Send( Publisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DetectionReport( const DetectionReport& );            //!< Copy constructor
    DetectionReport& operator=( const DetectionReport& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Agent_ABC& entity_;
    const dispatcher::Agent_ABC& detected_;
    int level_;
    //@}
};

}
}

#endif // __DetectionReport_h_
