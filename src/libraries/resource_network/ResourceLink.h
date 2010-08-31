// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ResourceLink_h_
#define __ResourceLink_h_

#include <vector>

namespace MsgsSimToClient
{
    class MsgUrbanAttributes_Infrastructures_ResourceNetwork_Link;
}

namespace xml
{
    class xistream;
}

namespace resource
{

// =============================================================================
/** @class  ResourceLink
    @brief  Resource link
*/
// Created: JSR 2010-08-13
// =============================================================================
class ResourceLink
{
public:
    enum EDestinationKind
    {
        eDestinationKindUrban,
        eDestinationKindObject
    };

public:
    //! @name Constructors/Destructor
    //@{
             ResourceLink( unsigned int destination, EDestinationKind kind, int capacity );
             ResourceLink( const ResourceLink& from );
    virtual ~ResourceLink();
    //@}

    //! @name Operations
    //@{
    static EDestinationKind FindDestinationKind( const std::string& kind );
    void SetCapacity( int capacity );
    int GetEfficientCapacity() const;
    unsigned int GetDestination() const;
    EDestinationKind GetDestinationKind() const;
    void SetFlow( unsigned int flow );
    //@}

    //! @name Network
    //@{
    void Serialize( MsgsSimToClient::MsgUrbanAttributes_Infrastructures_ResourceNetwork_Link& msg ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ResourceLink& operator=( const ResourceLink& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned int destination_;
    EDestinationKind kind_;
    int capacity_;
    unsigned int flow_;
    //@}
};

}

#endif // __ResourceLink_h_
