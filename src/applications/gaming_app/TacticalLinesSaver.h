// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TacticalLinesSaver_h_
#define __TacticalLinesSaver_h_

class LimitsModel;

// =============================================================================
/** @class  TacticalLinesSaver
    @brief  TacticalLinesSaver
*/
// Created: AGE 2006-09-20
// =============================================================================
class TacticalLinesSaver : public QObject
{

public:
    //! @name Constructors/Destructor
    //@{
             TacticalLinesSaver( QObject* parent, LimitsModel& model );
    virtual ~TacticalLinesSaver();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TacticalLinesSaver( const TacticalLinesSaver& );            //!< Copy constructor
    TacticalLinesSaver& operator=( const TacticalLinesSaver& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    bool SaveLimits();
    void LoadLimits();
    //@}

private:
    //! @name Member data
    //@{
    LimitsModel& model_;
    //@}
};

#endif // __TacticalLinesSaver_h_
