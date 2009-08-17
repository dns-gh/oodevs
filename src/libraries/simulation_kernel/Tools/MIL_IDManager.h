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
    @brief  MIL_IDManager
*/
// Created: SBO 2009-05-26
// =============================================================================
class MIL_IDManager
{

public:
    //! @name Operations
    //@{
    static unsigned long GetFreeId();//@TODO replace static by member function and give instance of MIL_IDManager to factories constructor

    template< typename Archive >
    static void serialize( Archive& archive, uint version );
    //@}

    //! @name Constructors/Destructor
    //@{
     MIL_IDManager();
    ~MIL_IDManager();
    //@}

private:

    //! @name Copy/Assignment
    //@{
    MIL_IDManager( const MIL_IDManager& );            //!< Copy constructor
    MIL_IDManager& operator=( const MIL_IDManager& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    static unsigned long last_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: MIL_IDManager::serialize
// Created: SBO 2009-05-26
// -----------------------------------------------------------------------------
template< typename Archive >
void MIL_IDManager::serialize( Archive& archive, uint /*version*/ )
{
    archive & last_;
}


#endif // __MIL_IDManager_h_
