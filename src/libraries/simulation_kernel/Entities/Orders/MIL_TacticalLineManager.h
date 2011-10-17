// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_TacticalLineManager_h_
#define __MIL_TacticalLineManager_h_

#include "MIL.h"
#include "MT_Tools/MT_Vector2DTypes.h"

class MIL_Formation;
class MIL_Automate;
class TER_LimitData;

// =============================================================================
/** @class  MIL_TacticalLineManager
    @brief  MIL_TacticalLineManager
*/
// Created: NLD 2006-11-10
// =============================================================================
class MIL_TacticalLineManager
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_TacticalLineManager();
    virtual ~MIL_TacticalLineManager();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    const TER_LimitData& CreateLimitData ( const T_PointVector& points );
    void  DestroyLimitData( const TER_LimitData& data );
    static MIL_TacticalLineManager& GetSingleton();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_TacticalLineManager( const MIL_TacticalLineManager& );            //!< Copy constructor
    MIL_TacticalLineManager& operator=( const MIL_TacticalLineManager& ); //!< Assignment operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< T_PointVector, TER_LimitData* > T_LimitDataMap;
    typedef T_LimitDataMap::iterator                  IT_LimitDataMap;
    typedef T_LimitDataMap::const_iterator            CIT_LimitDataMap;
    //@}

private:
    //! @name Member data
    //@{
    T_LimitDataMap limitsData_;
    //@}
};

#endif // __MIL_TacticalLineManager_h_
