// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AggregateMarking_h_
#define __AggregateMarking_h_

// =============================================================================
/** @class  AggregateMarking
    @brief  AggregateMarking
*/
// Created: AGE 2008-02-21
// =============================================================================
class AggregateMarking
{

public:
    //! @name Constructors/Destructor
    //@{
             AggregateMarking();
    virtual ~AggregateMarking();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AggregateMarking( const AggregateMarking& );            //!< Copy constructor
    AggregateMarking& operator=( const AggregateMarking& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    unsigned char markingType_;
    unsigned char markingData_[31];
    //@}
};

#endif // __AggregateMarking_h_
