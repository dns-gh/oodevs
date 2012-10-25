// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __IconsGaugeType_h_
#define __IconsGaugeType_h_

#include "GaugeType.h"
#pragma warning( push, 0 )
#include <QtGui/qpixmap.h>
#pragma warning( pop )

namespace indicators
{

// =============================================================================
/** @class  IconsGaugeType
    @brief  IconsGaugeType
*/
// Created: SBO 2009-05-05
// =============================================================================
class IconsGaugeType : public GaugeType
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit IconsGaugeType( xml::xistream& xis );
    virtual ~IconsGaugeType();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer, double key ) const;
    virtual void Display( QTreeWidgetItem* item, kernel::DisplayExtractor_ABC& extractor, int col, double value ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void LoadIcons( xml::xistream& xis );
    void LoadIcon( xml::xistream& xis, const std::string& path );
    QPixmap Icon( double value ) const;
    //@}

    //! @name Types
    //@{
    typedef std::map< double, QPixmap > T_Icons;
    //@}

private:
    //! @name Member data
    //@{
    T_Icons icons_;
    //@}
};

}

#endif // __IconsGaugeType_h_
