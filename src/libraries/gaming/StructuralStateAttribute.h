// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __StructuralStateAttribute_h_
#define __StructuralStateAttribute_h_

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Updatable_ABC.h"

namespace kernel
{
    class Controller;
    class PropertiesDictionary;
}

namespace MsgsSimToClient
{
    class MsgUrbanCreation;
    class MsgUrbanUpdate;
}

// =============================================================================
/** @class  StructuralStateAttribute
    @brief  StructuralStateAttribute
*/
// Created: JSR 2010-09-01
// =============================================================================
class StructuralStateAttribute : public kernel::StructuralStateAttribute_ABC
                               , public kernel::Updatable_ABC< MsgsSimToClient::MsgUrbanUpdate >
                               , public kernel::Updatable_ABC< MsgsSimToClient::MsgUrbanCreation >
{
public:
    //! @name Constructors/Destructor
    //@{
             StructuralStateAttribute( kernel::Controller& controller, kernel::PropertiesDictionary& dictionary );
    virtual ~StructuralStateAttribute();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    StructuralStateAttribute( const StructuralStateAttribute& );            //!< Copy constructor
    StructuralStateAttribute& operator=( const StructuralStateAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dico ) const;
    virtual void DoUpdate( const MsgsSimToClient::MsgUrbanUpdate& message );
    virtual void DoUpdate( const MsgsSimToClient::MsgUrbanCreation& message );
    template< typename T >
    void UpdateData( const T& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    unsigned int structuralState_;
    //@}
};

#endif // __StructuralStateAttribute_h_
