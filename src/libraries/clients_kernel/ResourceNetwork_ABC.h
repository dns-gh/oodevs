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

namespace MsgsSimToClient
{
    class MsgUrbanAttributes_Infrastructures;
}

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
{
public:
    //! @name Types
    //@{
    struct ResourceLink
    {
        ResourceLink()
            : urban_( true )
            , id_( 0 )
            , capacity_( -1 )
            , flow_( 0 )
        {}

        bool urban_;
        unsigned int id_;
        int capacity_;
        unsigned int flow_;
    };

    struct ResourceNode
    {
        ResourceNode()
            : isEnabled_( true )
            , production_( 0 )
            , consumption_( 0 )
            , critical_( false )
            , maxStock_( 0 )
            , stock_( 0 )
            , totalFlow_( 0 )
            , resource_()
        {}

        bool isEnabled_;
        unsigned int production_;
        unsigned int consumption_;
        bool critical_;
        unsigned int maxStock_;
        unsigned int stock_;
        unsigned int totalFlow_;
        std::string resource_;
        std::vector< ResourceLink > links_;
    };

    typedef std::map< std::string, ResourceNode > ResourceNodes;
    typedef ResourceNodes::const_iterator       CIT_ResourceNodes;
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
    void Select( bool selected ) { selected_ = selected; }
    bool IsSelected() const { return selected_; }
    const ResourceNodes& GetResourceNodes() const { return resourceNodes_; }
    ResourceNodes& GetResourceNodes() { return resourceNodes_; }
    const ResourceNode* FindResourceNode( std::string resource ) const
    {
        CIT_ResourceNodes it = resourceNodes_.find( resource );
        if( it == resourceNodes_.end() )
            return 0;
        return &it->second;
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ResourceNetwork_ABC( const ResourceNetwork_ABC& );            //!< Copy constructor
    ResourceNetwork_ABC& operator=( const ResourceNetwork_ABC& ); //!< Assignment operator
    //@}

protected:
    //! @name Member data
    //@{
    ResourceNodes resourceNodes_;
    bool selected_;
    //@}
};

}

#endif // __ResourceNetwork_ABC_h_
