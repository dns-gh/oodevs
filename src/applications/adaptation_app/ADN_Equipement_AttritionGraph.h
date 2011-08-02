// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Equipement_AttritionGraph_h_
#define __ADN_Equipement_AttritionGraph_h_

#include "ADN_Gfx_ABC.h"
#include "ADN_Equipement_Data.h"
#include "ADN_Categories_Data.h"
#include "ADN_AttritionInfos.h"
#include <QtGui/qwidget.h>

// =============================================================================
/** @class  ADN_Equipement_AttritionGraph
    @brief  ADN_Equipement_AttritionGraph
*/
// Created: JSR 2010-04-29
// =============================================================================
class ADN_Equipement_AttritionGraph : public QWidget , public ADN_Gfx_ABC
{
    Q_OBJECT

public:
    class GraphData
    {
    public:
        GraphData( const int number )
        {
            for( int i = 0; i < number; ++i )
                values_.push_back( i == number - 1 ? 100. : 0 );
        }

        typedef std::vector < double >      T_Values;
        typedef T_Values::iterator         IT_Values;
        typedef T_Values::const_iterator  CIT_Values;

        typedef std::vector < QRect >          T_Rectangles;
        typedef T_Rectangles::const_iterator CIT_Rectangles;

        unsigned int value_;
        T_Values     values_;
        QString      strName_;
        T_Rectangles rectangles_;
    };

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Equipement_AttritionGraph( QWidget* pParent = 0 );
    virtual ~ADN_Equipement_AttritionGraph();
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
    typedef std::vector < GraphData >   T_Columns;
    typedef T_Columns::iterator        IT_Columns;
    typedef T_Columns::const_iterator CIT_Columns;
    T_Columns columns_;

    typedef std::vector< helpers::AttritionInfos* > T_Attritions;
    typedef T_Attritions::iterator                             IT_Attritions;
    T_Attritions attritions_;

    std::vector < QColor > effectColors_;
    std::vector < QString > effectStrings_;
    //@}
};

#endif // __ADN_Equipement_AttritionGraph_h_
