// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FolkModel_h_
#define __FolkModel_h_

#include "game_asn/Asn.h"
#include "Entity_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  FolkModel
    @brief  FolkModel
*/
// Created: AGE 2007-09-04
// =============================================================================
class FolkModel : public Entity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             FolkModel();
    virtual ~FolkModel();
    //@}

    //! @name Operations
    //@{
    void Update( const ASN1T_MsgFolkCreation& message );
    virtual void SendFullUpdate( ClientPublisher_ABC& publisher ) const;
    virtual void SendCreation  ( ClientPublisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FolkModel( const FolkModel& );            //!< Copy constructor
    FolkModel& operator=( const FolkModel& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< std::string > T_Names;
    typedef T_Names::const_iterator  CIT_Names;
    //@}

private:
    //! @name Member data
    //@{
    T_Names profiles_, activities_;
    int container_size, edge_number;
    //@}
};

}

#endif // __FolkModel_h_
