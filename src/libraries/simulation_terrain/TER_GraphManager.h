// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TER_GraphManager_h_
#define __TER_GraphManager_h_

#include "TER_StaticData.h"

// =============================================================================
/** @class  TER_GraphManager
    @brief  %TER_GraphManager
*/
// Created: CMA 2011-08-16
// =============================================================================
class TER_GraphManager
{
public:
    //! @name Constructors/Destructor
    //@{
             TER_GraphManager( const std::string& strGraphArchive, const std::string& strNodeArchive, const std::string& strLinkArchive, float precision );
    virtual ~TER_GraphManager();
    //@}

    //! @name Cast to member
    //@{
    operator const TER_StaticData&() const { return staticData_; }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TER_GraphManager( const TER_GraphManager& );            //!< Copy constructor
    TER_GraphManager& operator=( const TER_GraphManager& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    TER_StaticData staticData_;
    //@}
};

#endif // __TER_GraphManager_h_
