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
#include <boost/serialization/split_member.hpp>

namespace Common
{
    class ResourceNetwork_Link;
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
    enum ETargetKind
    {
        eTargetKindUrban,
        eTargetKindObject
    };

public:
    //! @name Constructors/Destructor
    //@{
             ResourceLink();
             ResourceLink( unsigned int target, ETargetKind kind, int capacity );
             ResourceLink( const ResourceLink& from );
    virtual ~ResourceLink();
    //@}

    //! @name Operations
    //@{
    static ETargetKind FindTargetKind( const std::string& kind );
    void SetCapacity( int capacity );
    int GetEfficientCapacity() const;
    unsigned int GetTarget() const;
    ETargetKind GetTargetKind() const;
    void SetFlow( unsigned int flow );
    //@}

    //! @name Network
    //@{
    void Serialize( Common::ResourceNetwork_Link& msg ) const;
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    template< typename Archive >
    void load( Archive&, const unsigned int );
    template< typename Archive >
    void save( Archive&, const unsigned int ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ResourceLink& operator=( const ResourceLink& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned int target_;
    ETargetKind kind_;
    int capacity_;
    unsigned int flow_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ResourceLink::load
// Created: JSR 2010-11-17
// -----------------------------------------------------------------------------
template< typename Archive >
void ResourceLink::load( Archive& file, const unsigned int )
{
    file >> target_
         >> kind_
         >> capacity_;
}

// -----------------------------------------------------------------------------
// Name: ResourceLink::save
// Created: JSR 2010-11-17
// -----------------------------------------------------------------------------
template< typename Archive >
void ResourceLink::save( Archive& file, const unsigned int ) const
{
    file << target_
         << kind_
         << capacity_;
}

}

#endif // __ResourceLink_h_
