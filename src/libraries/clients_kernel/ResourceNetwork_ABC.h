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

#include "clients_kernel/Extension_ABC.h"
#include "resource_network/Types.h"

namespace MsgsSimToClient
{
    class MsgUrbanAttributes_Infrastructures;
}

namespace kernel
{

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
        bool urban_;
        unsigned int id_;
        int capacity_;
        unsigned int flow_;
    };

    struct ResourceNode
    {
        bool isEnabled_;
        unsigned int production_;
        unsigned int consumption_;
        bool critical_;
        unsigned int maxStock_;
        unsigned int stock_;
        unsigned int totalFlow_;
        resource::E_ResourceType type_;
        std::vector< ResourceLink > links_;    
    };

    typedef std::map< resource::E_ResourceType, ResourceNode > ResourceNodes;
    typedef ResourceNodes::const_iterator                  CIT_ResourceNodes;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             ResourceNetwork_ABC() : selected_( false ) {}
    virtual ~ResourceNetwork_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual QString GetLinkName( resource::E_ResourceType type, unsigned int i ) const = 0;
    void Select( bool selected ) { selected_ = selected; }
    bool IsSelected() const { return selected_; }
    const ResourceNode* FindResourceNode( resource::E_ResourceType type ) const
    {
        CIT_ResourceNodes it = resourceNodes_.find( type );
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
