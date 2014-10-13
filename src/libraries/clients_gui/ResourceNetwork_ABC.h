// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef CLIENTS_GUI_RESOURCE_NETWORK_H__
#define CLIENTS_GUI_RESOURCE_NETWORK_H__

#include "clients_kernel/Extension_ABC.h"
#include <geometry/Types.h>
#include <boost/noncopyable.hpp>

namespace gui
{
    class GLView_ABC;
    class Viewport_ABC;
}

namespace gui
{
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

        void ReplaceLinkId( unsigned long oldId, unsigned long newId )
        {
            if( links_.empty() )
                return;
            for( auto it = links_.begin(); it != links_.end(); ++it )
                if( it->id_ == oldId )
                    it->id_ = newId;
        }
    };

    typedef std::map< std::string, ResourceNode > T_ResourceNodes;

    struct ResourceLinkDeletion
    {
        std::string resource_;
        unsigned int id_;
        bool isUrban_;
    };

// =============================================================================
/** @class  ResourceNetwork_ABC
    @brief  ResourceNetwork_ABC
*/
// Created: JSR 2010-08-19
// =============================================================================
class ResourceNetwork_ABC : public kernel::Extension_ABC
                          , public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ResourceNetwork_ABC() : selected_( false ) {}
    virtual ~ResourceNetwork_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual QString GetLinkName( const std::string& resource, unsigned int i ) const = 0;
    virtual void Draw( const Viewport_ABC& viewport, const GLView_ABC& tools, const geometry::Point2f& position, float alpha ) const = 0;
    virtual ResourceNode& FindOrCreateResourceNode( const std::string& resource, unsigned int defaultProduction = 0 )
    {
        auto it = resourceNodes_.find( resource );
        if( it == resourceNodes_.end() )
        {
            resourceNodes_[ resource ].resource_ = resource;
            resourceNodes_[ resource ].production_ = defaultProduction;
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

#endif // CLIENTS_GUI_RESOURCE_NETWORK_H__
