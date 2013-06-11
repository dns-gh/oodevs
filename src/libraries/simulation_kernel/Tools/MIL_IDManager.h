// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_IDManager_h_
#define __MIL_IDManager_h_

// =============================================================================
/** @class  MIL_IDManager
    @brief  MIL id manager definition
*/
// Created: SBO 2009-05-26
// =============================================================================
class MIL_IDManager : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_IDManager();
    virtual ~MIL_IDManager();
    //@}

public:
    //! @name Operations
    //@{
    static void SetKeepIdsMode( bool bKeepIds, unsigned long lastIdBeforeKeepIds = 1 );
    unsigned long GetId( unsigned long wishedId = 0, bool bForceId = false );
    template< typename Archive >
    void serialize( Archive& archive, unsigned int version );
    //@}

private:
    //! @name Member data
    //@{    
    static unsigned long last_;
    static bool bKeepIds_;
    static std::set< unsigned long > ids_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: MIL_IDManager::serialize
// Created: SBO 2009-05-26
// -----------------------------------------------------------------------------
template< typename Archive >
void MIL_IDManager::serialize( Archive& archive, unsigned int /*version*/ )
{
    archive & last_;
}

#endif // __MIL_IDManager_h_
