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
class GlTools_ABC;
class Viewport_ABC;

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

    typedef std::vector< ResourceLink >       T_ResourceLinks;
    typedef T_ResourceLinks::iterator        IT_ResourceLinks;
    typedef T_ResourceLinks::const_iterator CIT_ResourceLinks;

    struct ResourceNode
    {
        ResourceNode()
            : isEnabled_   ( true )
            , production_  ( 0 )
            , consumption_ ( 0 )
            , needs_       ( 0 )
            , satisfaction_( 0.f )
            , critical_    ( false )
            , maxStock_    ( 0 )
            , stock_       ( 0 )
            , totalFlow_   ( 0 )
        {}

        bool isEnabled_;
        unsigned int production_;
        unsigned int consumption_;
        unsigned int needs_;
        float satisfaction_;
        bool critical_;
        unsigned int maxStock_;
        unsigned int stock_;
        unsigned int totalFlow_;
        std::string resource_;
        T_ResourceLinks links_;
    };

    typedef std::map< std::string, ResourceNode > T_ResourceNodes;
    typedef T_ResourceNodes::iterator            IT_ResourceNodes;
    typedef T_ResourceNodes::const_iterator     CIT_ResourceNodes;
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
    virtual ResourceNode& FindOrCreateResourceNode( std::string resource )
    {
        IT_ResourceNodes it = resourceNodes_.find( resource );
        if( it == resourceNodes_.end() )
        {
            resourceNodes_[ resource ].resource_ = resource;
            return resourceNodes_[ resource ];
        }
        return it->second;
    }
    void Select( bool selected ) { selected_ = selected; }
    bool IsSelected() const { return selected_; }
    const T_ResourceNodes& GetResourceNodes() const { return resourceNodes_; }
    // $$$$ _RC_ JSR 2011-02-25: Supprimer le getter non const
    T_ResourceNodes& GetResourceNodes() { return resourceNodes_; }
    const ResourceNode* FindResourceNode( std::string resource ) const
    {
        CIT_ResourceNodes it = resourceNodes_.find( resource );
        if( it == resourceNodes_.end() )
            return 0;
        return &it->second;
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
