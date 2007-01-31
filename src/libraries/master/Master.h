// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Master_h_
#define __Master_h_

#include <memory>

namespace master
{
class Config;
class ClientsNetworker;
class DataManager;
class ProfileManager;

// =============================================================================
/** @class  Master
    @brief  Master
*/
// Created: NLD 2007-01-24
// =============================================================================
class Master
{
public:
    //! @name Constructors/Destructor
    //@{
     Master( Config& config );
    ~Master();
    //@}

    //! @name Operations
    //@{
    void Update();
    //@}

    //! @name Accessors
    //@{
    const DataManager&    GetDataManager   () const;
    const ProfileManager& GetProfileManager() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Master( const Master& );            //!< Copy constructor
    Master& operator=( const Master& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< ClientsNetworker > clientsNetworker_;
    std::auto_ptr< DataManager      > dataManager_;
    std::auto_ptr< ProfileManager   > profileManager_;
    //@}
};

}

#endif // __Master_h_
