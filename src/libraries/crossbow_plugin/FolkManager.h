// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __crossbow_FolkManager_h_
#define __crossbow_FolkManager_h_

#include "game_asn/Asn.h"
#include <vector>

namespace dispatcher
{
    class Config;
}

namespace crossbow
{    
// =============================================================================
/** @class  FolkManager
    @brief  FolkManager
*/
// Created: JCR 2007-08-28
// =============================================================================
class FolkManager
{
    //! @name Types
    //@{
    typedef std::vector<std::string>    T_Trait;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit FolkManager( const dispatcher::Config& config );
    virtual ~FolkManager();
    //@}

    //! @name Operations
    //@{    
    void Send( const ASN1T_MsgFolkCreation& asn );    
    //@}

    //! @name Accessors
    //@{
    unsigned Filter( const ASN1T_MsgFolkGraphEdgeUpdate& asn, /*out*/ std::vector<unsigned>& container ) const;    
    //@}

private:
    //! @name Copy/Assignement
    //@{
    FolkManager( const FolkManager& );            //!< Copy constructor
    FolkManager& operator=( const FolkManager& ); //!< Assignement operator
    //@}

    //! @name 
    //@{
    template< typename MsgFolkManagerCreation_trait >
    void Update ( T_Trait& trait, const MsgFolkManagerCreation_trait& asn );
    //@}

private:
    //! @name Member data
    //@{    
    T_Trait     profiles_;
    T_Trait     activities_;
    unsigned    container_size_;
    //@}
};

}

#endif // __dispatcher_FolkManager_h_
