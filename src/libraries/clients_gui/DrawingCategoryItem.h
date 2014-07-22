// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DrawingCategoryItem_h_
#define __DrawingCategoryItem_h_

namespace gui
{
    class DrawingCategory;
    class DrawingTemplate;

// =============================================================================
/** @class  DrawingCategoryItem
    @brief  DrawingCategoryItem
*/
// Created: SBO 2008-06-04
// =============================================================================
class DrawingCategoryItem : public Q3VButtonGroup
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             DrawingCategoryItem( QWidget* parent, const DrawingCategory& category );
    virtual ~DrawingCategoryItem();
    //@}

    //! @name Operations
    //@{
    void ClearSelection();
    //@}

signals:
    //! @name Signals
    //@{
    void Selected( const DrawingTemplate& style );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnClicked( int id );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DrawingCategoryItem( const DrawingCategoryItem& );            //!< Copy constructor
    DrawingCategoryItem& operator=( const DrawingCategoryItem& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void FillStyles( const DrawingCategory& category );
    void AddStyle( const DrawingTemplate& style );
    //@}

    //! @name Types
    //@{
    typedef std::map< int, const DrawingTemplate* > T_Styles;
    //@}

private:
    //! @name Member data
    //@{
    T_Styles styles_;
    //@}
};

}

#endif // __DrawingCategoryItem_h_
