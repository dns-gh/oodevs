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

#include "protocol/protocol.h"

using namespace Common;
using namespace MsgsSimToClient;

namespace plugins
{
namespace crossbow
{
    class Table_ABC;
    class Row_ABC;
    class Workspace_ABC;
    class Database_ABC;
    class WorkingSession;

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
    explicit FolkUpdater( Workspace_ABC& workspace, const WorkingSession& session );
    virtual ~FolkUpdater();
    //@}

    //! @name
    //@{
    void Lock();
    void UnLock();
    //@}

    //! @name Operations
    //@{
    void Update( const MsgFolkCreation& msg );
    void Update( const MsgFolkGraphUpdate& msg );
    void Drop();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FolkUpdater( const FolkUpdater& );            //!< Copy constructor
    FolkUpdater& operator=( const FolkUpdater& ); //!< Assignment operator
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
    void Update( const MsgFolkGraphEdgeUpdate& msg );
    void Update( Edge& edge, const MsgFolkGraphEdgeUpdate& msg ) const;
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
    const WorkingSession& session_;
    //@}
};

}
}

#endif // __crossbow_FolkUpdater_h_
