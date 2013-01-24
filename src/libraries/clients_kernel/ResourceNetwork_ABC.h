// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ResourceNetwork_ABC_h_
#define __ResourceNetwork_ABC_h_

#include "Extension_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
// =============================================================================
/** @class  ResourceNetwork_ABC
    @brief  ResourceNetwork_ABC
*/
// Created: JSR 2010-08-19
// =============================================================================
class ResourceNetwork_ABC : public Extension_ABC
                          , private boost::noncopyable
{
public:
    //! @name Types
    //@{
    struct ResourceLink
    {
        ResourceLink( bool urban = true, unsigned int id = 0 )
            : urban_   ( urban )
            , id_      ( id )
            , capacity_( -1 )
            , flow_    ( 0 )
        {}

        bool urban_;
        unsigned int id_;
        int capacity_;
        unsigned int flow_;
    };

    typedef std::vector< ResourceLink > T_ResourceLinks;

    struct ResourceNode
    {
        ResourceNode()
            : isEnabled_   ( true )
            , critical_    ( false )
            , production_  ( 0 )
            , consumption_ ( 0 )
            , needs_       ( 0 )
            , satisfaction_( 0.f )
            , maxStock_    ( 0 )
            , stock_       ( 0 )
            , totalFlow_   ( 0 )
        {}

        bool isEnabled_;
        bool critical_;
        unsigned int production_;
        unsigned int consumption_;
        unsigned int needs_;
        float satisfaction_;
        unsigned int maxStock_;
        unsigned int stock_;
        unsigned int totalFlow_;
        std::string resource_;
        T_ResourceLinks links_;
    };

    typedef std::map< std::string, ResourceNode > T_ResourceNodes;

    struct Deletion
    {
        std::string resource_;
        unsigned int id_;
        bool isUrban_;
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             ResourceNetwork_ABC() : selected_( false ) {}
    virtual ~ResourceNetwork_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual QString GetLinkName( const std::string& resource, unsigned int i ) const = 0;
    virtual void Draw( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const = 0;
    virtual ResourceNode& FindOrCreateResourceNode( const std::string& resource )
    {
        auto it = resourceNodes_.find( resource );
        if( it == resourceNodes_.end() )
        {
            resourceNodes_[ resource ].resource_ = resource;
            return resourceNodes_[ resource ];
        }
        return it->second;
    }
    virtual void RemoveNode( const std::string& resource )
    {
        resourceNodes_.erase( resource );
    }
    void Select( bool selected ) { selected_ = selected; }
    bool IsSelected() const { return selected_; }
    const T_ResourceNodes& GetResourceNodes() const { return resourceNodes_; }
    // $$$$ _RC_ JSR 2011-02-25: Supprimer le getter non const
    T_ResourceNodes& GetResourceNodes() { return resourceNodes_; }
    const ResourceNode* FindResourceNode( std::string resource ) const
    {
        auto it = resourceNodes_.find( resource );
        if( it == resourceNodes_.end() )
            return 0;
        return &it->second;
    }
    void RemoveLinks( bool urban, unsigned int id, const std::string& resource = std::string() )
    {
        for( auto it = resourceNodes_.begin(); it != resourceNodes_.end(); ++it )
        {
            if( !resource.empty() && resource != it->first )
                continue;
            T_ResourceLinks& links = it->second.links_;
            if( !links.empty() )
            {
                auto link = links.begin();
                while( link != links.end() )
                {
                    if( link->id_ == id && link->urban_ == urban )
                        link = links.erase( link );
                    else
                        ++link;
                }
            }
        }
    }
    //@}

protected:
    //! @name Member data
    //@{
    T_ResourceNodes resourceNodes_;
    bool selected_;
    //@}
};

}

#endif // __ResourceNetwork_ABC_h_
