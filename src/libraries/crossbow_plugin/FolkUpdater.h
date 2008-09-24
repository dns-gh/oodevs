// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __crossbow_FolkUpdater_h_
#define __crossbow_FolkUpdater_h_

#include "game_asn/Simulation.h"

namespace plugins
{
namespace crossbow
{    
    class Table_ABC;
    class Row_ABC;
    class Database_ABC;

// =============================================================================
/** @class  FolkUpdater
    @brief  FolkUpdater
*/
// Created: JCR 2007-08-29
// =============================================================================
class FolkUpdater
{    
public:
    //! @name Constructors/Destructor
    //@{
    explicit FolkUpdater( Database_ABC& database );
    virtual ~FolkUpdater();
    //@}

    //! @name 
    //@{
    void Lock();
    void UnLock();
    //@}

    //! @name Operations
    //@{
    void Update( const ASN1T_MsgFolkCreation& msg );
    void Update( const ASN1T_MsgFolkGraphUpdate& msg );    
    void Drop();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    FolkUpdater( const FolkUpdater& );            //!< Copy constructor
    FolkUpdater& operator=( const FolkUpdater& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    struct Edge
    {
        Edge() : population_( 0 ), containers_( 5, 0 ) {}
        unsigned population_;
        std::vector< unsigned > containers_;
    };

    typedef std::vector< Edge > T_Edges;
    typedef T_Edges::const_iterator CIT_Edges;
    //@}

    //! @name Helpers
    //@{
    void Commit( Table_ABC& table );
    void CommitEdge( Row_ABC& row, const Edge& edge );
    void Update( Table_ABC& table, const ASN1T_MsgFolkGraphUpdate& msg );
    void Update( Table_ABC& table, const ASN1T_MsgFolkGraphEdgeUpdate& msg );
    void Update( Edge& edge, const ASN1T_MsgFolkGraphEdgeUpdate& msg ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::vector< std::string > activities_;
    std::vector< std::string > profiles_;
    T_Edges edges_;
    unsigned long updated_;
    //@}

    //! @name Folk Database
    //@{
    Database_ABC& database_;
    //@}
};

}
}

#endif // __crossbow_FolkUpdater_h_
