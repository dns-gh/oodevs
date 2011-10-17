// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __FuneralHandler_ABC_h_
#define __FuneralHandler_ABC_h_

namespace logistic {
    class LogisticHierarchy_ABC;
    class FuneralConsign_ABC;
    class FuneralPackagingResource;
    class SupplyConvoysObserver_ABC;

// =============================================================================
/** @class  FuneralConsign_ABC
    @brief  FuneralConsign_ABC
*/
// Created: NLD 2011-08-24
// =============================================================================
class FuneralHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FuneralHandler_ABC() {}
    virtual ~FuneralHandler_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual const MT_Vector2D&           GetPosition() const = 0;
    virtual const LogisticHierarchy_ABC& GetLogisticHierarchy() const = 0;

    virtual bool                            FuneralHandleConsign           ( boost::shared_ptr< FuneralConsign_ABC > consign ) = 0;
    virtual const FuneralPackagingResource* FuneralGetNextPackagingResource( const FuneralPackagingResource* currentPackaging ) = 0;

    virtual void AddSupplyConvoysObserver   ( SupplyConvoysObserver_ABC& observer ) = 0;
    virtual void RemoveSupplyConvoysObserver( SupplyConvoysObserver_ABC& observer ) = 0;
    //@}

    //! @name Network
    //@{
    virtual void Serialize( sword::ParentEntity& message ) const = 0;
    //@}
};

} // end namespace logistic

#endif // __FuneralHandler_ABC_h_
