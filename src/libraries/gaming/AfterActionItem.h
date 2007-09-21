// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AfterActionItem_h_
#define __AfterActionItem_h_

#include "AfterActionItem_ABC.h"

// =============================================================================
/** @class  AfterActionItem
    @brief  AfterActionItem
*/
// Created: AGE 2007-09-19
// =============================================================================
class AfterActionItem : public AfterActionItem_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AfterActionItem( const std::string& type, xml::xistream& xis );
    virtual ~AfterActionItem();
    //@}

    //! @name Operations
    //@{
    virtual void Commit( xml::xostream& xos ) const;

    virtual void Build     ( AfterActionBuilder_ABC& builder ) const;
    virtual void Connect   ( xml::xistream& xis, kernel::Resolver_ABC< AfterActionItem_ABC, std::string >& items );
    virtual void Connect   ( int input, AfterActionItem_ABC& rhs );
    virtual void ConnectOutput( AfterActionItem_ABC& rhs, int input );
    virtual bool CanConnect( int input, const AfterActionItem_ABC* rhs = 0 ) const;
    virtual void Disconnect( AfterActionItem_ABC* item );
    virtual void Disconnect( AfterActionItem_ABC* item, int input );
    virtual std::string Resolve( const AfterActionItem_ABC* connection ) const;
    virtual std::string Resolve( int input, const AfterActionItem_ABC* ignore ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AfterActionItem& operator=( const AfterActionItem& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    int LinkOutput() const;
    std::string ConnectedOutputType( const AfterActionItem_ABC* ignore ) const;
    bool IsFree( int input ) const;
    virtual bool IsCompatible( const std::string& type, const AfterActionItem_ABC* connection ) const;
    std::string BuildType() const;
    //@}

    //! @name Types
    //@{
    typedef std::vector< std::string >                     T_Inputs;
    typedef std::vector< AfterActionItem_ABC* >            T_Connections;
    typedef T_Connections::iterator                       IT_Connections;
    typedef std::pair< AfterActionItem_ABC*, int >         T_OutgoingConnection;
    typedef std::vector< T_OutgoingConnection >            T_OutgoingConnections;
    typedef T_OutgoingConnections::iterator               IT_OutgoingConnections;
    typedef T_OutgoingConnections::const_iterator        CIT_OutgoingConnections;
    //@}

private:
    //! @name Member data
    //@{
    const std::string type_;
    const std::string name_;
    const std::string output_;
    T_Inputs              inputs_;
    T_Connections         inputConnections_;
    T_OutgoingConnections outputConnections_;
    //@}
};

#endif // __AfterActionItem_h_
