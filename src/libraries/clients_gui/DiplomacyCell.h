// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DiplomacyCell_h_
#define __DiplomacyCell_h_

namespace kernel
{
    class Karma;
}

namespace gui
{

// =============================================================================
/** @class  DiplomacyCell
    @brief  DiplomacyCell
*/
// Created: SBO 2006-09-25
// =============================================================================
class DiplomacyCell : public QObject
                    , public Q3ComboTableItem
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    explicit DiplomacyCell( Q3Table* table );
    virtual ~DiplomacyCell();
    //@}

    //! @name Operations
    //@{
    kernel::Karma GetValue() const;
    //@}

private slots:
    //! @name private slots
    //@{
    void OnTextChanged( const QString& );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DiplomacyCell( const DiplomacyCell& );            //!< Copy constructor
    DiplomacyCell& operator=( const DiplomacyCell& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void paint( QPainter* p, const QColorGroup& cg, const QRect& cr, bool selected );
    virtual QWidget* createEditor() const;
    void SetColor( const kernel::Karma& karma, const QColor& color );
    //@}

private:
    std::map< const QString, QColor > colors_;
    std::map< const QString, kernel::Karma > diplomacies_;
    bool valueSet_;
};

}

#endif // __DiplomacyCell_h_
