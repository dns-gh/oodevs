// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameters_h_
#define __MissionParameters_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "gaming/Types.h"
#include "ASN_Types.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

// =============================================================================
/** @class  MissionParameters
    @brief  Mission parameters
*/
// Created: SBO 2006-11-13
// =============================================================================
class MissionParameters : public kernel::Extension_ABC
                        , public kernel::Updatable_ABC< ASN1T_MsgPionOrder >
                        , public kernel::Updatable_ABC< ASN1T_MsgPionOrderManagement >
                        , public kernel::Updatable_ABC< ASN1T_MsgAutomateOrder >
                        , public kernel::Updatable_ABC< ASN1T_MsgAutomateOrderManagement >
                        , public kernel::Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MissionParameters( const kernel::CoordinateConverter_ABC& converter );
    virtual ~MissionParameters();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MissionParameters( const MissionParameters& );            //!< Copy constructor
    MissionParameters& operator=( const MissionParameters& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void Clear();
    void DecodePointList( const ASN1T_Line& src, T_PointVector& dest );
    void AddLima( const ASN1T_LimaOrder& lima );
    
    virtual void DoUpdate( const ASN1T_MsgPionOrder& message );
    virtual void DoUpdate( const ASN1T_MsgPionOrderManagement& message );
    virtual void DoUpdate( const ASN1T_MsgAutomateOrder& message );
    virtual void DoUpdate( const ASN1T_MsgAutomateOrderManagement& message );

    template< typename T >
    void DoUpdateOrderManagement( const T& message );
    void DoUpdateOrderContext( const ASN1T_OrderContext& context );
    //@}

    //! @name Types
    //@{
    typedef std::vector< E_FuncLimaType > T_LimaFunctions;
    typedef std::vector< std::pair< T_PointVector, T_LimaFunctions > > T_Limas;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    geometry::Rectangle2f boundingBox_;
    T_PointVector leftLimit_;
    T_PointVector rightLimit_;
    T_Limas limas_;
    //@}
};

#endif // __MissionParameters_h_
