// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterLimaList_h_
#define __ActionParameterLimaList_h_

#include "game_asn/Asn.h"
#include "ActionParameter.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

// =============================================================================
/** @class  ActionParameterLimaList
    @brief  ActionParameterLimaList
*/
// Created: SBO 2007-04-16
// =============================================================================
class ActionParameterLimaList : public ActionParameter< QString >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionParameterLimaList( const QString& name );
             ActionParameterLimaList( const QString& name, const kernel::CoordinateConverter_ABC& converter, const ASN1T_LimasOrder& limas );
    virtual ~ActionParameterLimaList();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    void AddLima( const T_PointVector& points, const QString& function );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterLimaList( const ActionParameterLimaList& );            //!< Copy constructor
    ActionParameterLimaList& operator=( const ActionParameterLimaList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    typedef std::pair< T_PointVector, QString > T_Lima;
    typedef std::vector< T_Lima > T_Limas;
    //@}

private:
    //! @name Member data
    //@{
    T_Limas limas_;
    geometry::Rectangle2f boundingBox_;
    //@}
};

#endif // __ActionParameterLimaList_h_
