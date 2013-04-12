// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Resources_AttritionGraph_h_
#define __ADN_Resources_AttritionGraph_h_

#include "ADN_Gfx_ABC.h"
#include "ADN_Resources_Data.h"
#include "ADN_Categories_Data.h"
#include "ADN_AttritionInfos.h"

// =============================================================================
/** @class  ADN_Resources_AttritionGraph
    @brief  ADN_Resources_AttritionGraph
*/
// Created: JSR 2010-04-29
// =============================================================================
class ADN_Resources_AttritionGraph : public QWidget , public ADN_Gfx_ABC
{
    Q_OBJECT

public:
    class GraphData
    {
    public:
        GraphData( const int number )
            : value_( 0 )
        {
            for( int i = 0; i < number; ++i )
                values_.push_back( i == number - 1 ? 100. : 0 );
        }

        typedef std::vector < double > T_Values;

        typedef std::vector < QRect > T_Rectangles;

        unsigned int value_;
        T_Values     values_;
        QString      strName_;
        T_Rectangles rectangles_;
    };

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Resources_AttritionGraph( QWidget* pParent = 0 );
    virtual ~ADN_Resources_AttritionGraph();
    //@}

    //! @name Operations
    //@{
    void AddAttrition( helpers::AttritionInfos& info );
    void RemoveAttrition( helpers::AttritionInfos& info );
    void ClearAttritions();
    void Update();
    QString GetTextTooltip( const QPoint& point, QRect& rc ) const;
    //@}

protected:
    //! @name Qt overloaded methods
    //@{
    virtual void paintEvent( QPaintEvent* );
    virtual bool event( QEvent* e );
    //@}

private:
    //! @name Member data
    //@{
    typedef std::vector < GraphData > T_Columns;
    typedef std::vector< helpers::AttritionInfos* > T_Attritions;

    T_Columns columns_;
    T_Attritions attritions_;
    std::vector < QColor > effectColors_;
    std::vector < QString > effectStrings_;
    //@}
};

#endif // __ADN_Resources_AttritionGraph_h_
