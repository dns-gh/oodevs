// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-02-01 $
// $Archive: /MVW_v10/Build/SDK/TER/src/TER_DynamicData.h $
// $Author: Age $
// $Modtime: 24/03/05 15:38 $
// $Revision: 3 $
// $Workfile: TER_DynamicData.h $
//
// *****************************************************************************

#ifndef __TER_DynamicData_h_
#define __TER_DynamicData_h_

#include "boost/shared_ptr.hpp"
class TerrainRetractationHandle;
namespace boost
{
    class mutex;
}

// =============================================================================
/** @class  TER_DynamicData
    @brief  TER_DynamicData
*/
// Created: AGE 2005-02-01
// =============================================================================
class TER_DynamicData
{
    friend class TER_PathFindManager;
public:
    //! @name Constructors/Destructor
    //@{
    virtual ~TER_DynamicData();
    //@}

    //! @name Operations
    //@{
    TER_DynamicData& Duplicate();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TER_DynamicData( const TER_DynamicData& );            //!< Copy constructor
    TER_DynamicData& operator=( const TER_DynamicData& ); //!< Assignement operator
    //@}

    //! @name Constructor
    //@{
    TER_DynamicData();
    //@}

    //! @name Operations
    //@{
    void AddRetractationHandle( TerrainRetractationHandle& handle );
    //@}

    //! @name Types
    //@{
    typedef boost::shared_ptr< TerrainRetractationHandle > T_Handle;
    typedef std::vector< T_Handle >                        T_Handles;
    typedef T_Handles::iterator                           IT_Handles;
    //@}

private:
    //! @name Member data
    //@{
    T_Handles handles_;
    //@}
};

#endif // __TER_DynamicData_h_
