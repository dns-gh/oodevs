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

#include <boost/serialization/split_member.hpp>
#include <vector>

namespace sword
{
    class ResourceNetwork_Link;
}

namespace xml
{
    class xistream;
    class xostream;
}

namespace resource
{
class ResourceTools_ABC;

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
    void Finalize( const ResourceTools_ABC& tools );
    void Prepare();
    static ETargetKind FindTargetKind( const std::string& kind );
    void SetCapacity( int capacity );
    int GetEfficientCapacity() const;
    unsigned int GetTarget() const;
    ETargetKind GetTargetKind() const;
    void ResetFlow();
    void SetFlow( unsigned int flow );
    bool NeedUpdate() const;
    //@}

    //! @name Network
    //@{
    void Serialize( sword::ResourceNetwork_Link& msg ) const;
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    template< typename Archive >
    void load( Archive&, const unsigned int );
    template< typename Archive >
    void save( Archive&, const unsigned int ) const;
    void WriteODB( xml::xostream& xos, const ResourceTools_ABC& tools );
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
    unsigned int oldFlow_;
    mutable bool needUpdate_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ResourceLink::load
// Created: JSR 2010-11-17
// -----------------------------------------------------------------------------
template< typename Archive >
void ResourceLink::load( Archive& file, const unsigned int )
{
    file >> target_;
    file >> kind_;
    file >> capacity_;
}

// -----------------------------------------------------------------------------
// Name: ResourceLink::save
// Created: JSR 2010-11-17
// -----------------------------------------------------------------------------
template< typename Archive >
void ResourceLink::save( Archive& file, const unsigned int ) const
{
    file << target_;
    file << kind_;
    file << capacity_;
}

}
#endif // __ResourceLink_h_
