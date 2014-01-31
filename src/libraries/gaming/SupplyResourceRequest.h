// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SupplyResourceRequest_h_
#define __SupplyResourceRequest_h_

namespace kernel
{
    class DotationType;
    class DisplayExtractor_ABC;
}

namespace sword
{
    class SupplyResourceRequest;
}

namespace google
{
namespace protobuf
{
    template< typename T > class RepeatedPtrField;
}
}

// =============================================================================
/** @class  SupplyResourceRequest
    @brief  SupplyResourceRequest
*/
// Created: AGE 2006-02-28
// =============================================================================
class SupplyResourceRequest
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyResourceRequest( const kernel::DotationType& type, unsigned int requested, unsigned int granted, unsigned int convoyed );
    virtual ~SupplyResourceRequest();
    //@}

public:
    //! @name Accessors
    //@{
    QString GetTypeName() const;
    unsigned int GetRequested() const;
    unsigned int GetGranted() const;
    unsigned int GetConvoyed() const;
    bool IsDelivered() const;
    void Deliver();
    void Update( const ::google::protobuf::RepeatedPtrField< ::sword::SupplyResourceRequest >& resources );
    //@}

    //! @name Member data
    //@{
    const kernel::DotationType* type_;
    unsigned int requested_;
    unsigned int granted_;
    unsigned int convoyed_;
    bool delivered_;
    //@}
};

#endif // __DotationRequest_h_
