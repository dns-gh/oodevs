// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PositionReport_h_
#define __PositionReport_h_

namespace dispatcher
{
    class Agent;
}

namespace bml
{
    class Publisher_ABC;

// =============================================================================
/** @class  PositionReport
    @brief  PositionReport
*/
// Created: SBO 2008-05-22
// =============================================================================
class PositionReport
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit PositionReport( const dispatcher::Agent& entity );
    virtual ~PositionReport();
    //@}

    //! @name Operations
    //@{
    void Send( Publisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PositionReport( const PositionReport& );            //!< Copy constructor
    PositionReport& operator=( const PositionReport& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Agent& entity_;
    //@}
};

}

#endif // __PositionReport_h_
