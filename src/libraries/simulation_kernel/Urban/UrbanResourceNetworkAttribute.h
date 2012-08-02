// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __UrbanResourceNetworkAttribute_h_
#define __UrbanResourceNetworkAttribute_h_

#include "UrbanExtension_ABC.h"
#include <boost/noncopyable.hpp>
#include <map>

// =============================================================================
/** @class  UrbanResourceNetworkAttribute
    @brief  UrbanResourceNetworkAttribute
*/
// Created: JSR 2012-08-02
// =============================================================================
class UrbanResourceNetworkAttribute : public UrbanExtension_ABC
                                    , private boost::noncopyable
{
public:
    //! @name Types
    //@{
    struct ResourceLink
    {
        ResourceLink()
            : id_( 0 )
            , capacity_( -1 )
        {}
        unsigned int id_;
        int capacity_;
    };

    struct ResourceNode
    {
        ResourceNode()
            : isEnabled_( true )
            , production_( 0 )
            , consumption_( 0 )
            , critical_( false )
            , maxStock_( 0 )
        {}
        bool isEnabled_;
        unsigned int production_;
        unsigned int consumption_;
        bool critical_;
        unsigned int maxStock_;
        std::string resource_;
        std::vector< ResourceLink > links_;
    };

    typedef std::map< std::string, ResourceNode > T_ResourceNodes;
    typedef T_ResourceNodes::iterator            IT_ResourceNodes;
    typedef T_ResourceNodes::const_iterator     CIT_ResourceNodes;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit UrbanResourceNetworkAttribute( xml::xistream& xis );
    virtual ~UrbanResourceNetworkAttribute();
    //@}

public:
    //! @name Operations
    //@{
    const T_ResourceNodes& GetResourceNodes() const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadNode( xml::xistream& xis );
    void ReadLink( xml::xistream& xis, ResourceNode& node );
    //@}

private:
    //! @name Member data
    //@{
    T_ResourceNodes resourceNodes_;
    //@}
};

#endif // __UrbanResourceNetworkAttribute_h_
