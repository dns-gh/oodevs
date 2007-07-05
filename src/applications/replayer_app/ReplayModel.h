// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ReplayModel_h_
#define __ReplayModel_h_

#include "dispatcher/ReplayModel_ABC.h"
#include <fstream>
#include <map>

// =============================================================================
/** @class  ReplayModel
    @brief  ReplayModel
*/
// Created: AGE 2007-07-05
// =============================================================================
class ReplayModel : public dispatcher::ReplayModel_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ReplayModel();
    virtual ~ReplayModel();
    //@}

    //! @name Operations
    //@{
    virtual void OnReceive( const ASN1T_MsgsSimToClient& asnMsg );
    virtual void StartSynchronisation();
    virtual void EndSynchronisation();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ReplayModel( const ReplayModel& );            //!< Copy constructor
    ReplayModel& operator=( const ReplayModel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    std::ofstream& GetArchive( unsigned int type );
    //@}

private:
    //! @name Member data
    //@{
    ASN1OCTET aASNEncodeBuffer_[100000];
    std::map< unsigned int, std::ofstream* > archives_;
    //@}
};

#endif // __ReplayModel_h_
