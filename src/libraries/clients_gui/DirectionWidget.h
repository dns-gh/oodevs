// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DirectionWidget_h_
#define __DirectionWidget_h_

namespace gui
{

// =============================================================================
/** @class  DirectionWidget
    @brief  DirectionWidget
*/
// Created: AGE 2007-01-02
// =============================================================================
class DirectionWidget : public QFrame
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             DirectionWidget( const QString& objectName, QWidget* parent );
    virtual ~DirectionWidget();
    //@}

    //! @name Operations
    //@{
    QString GetValue() const;
    void SetValue( const QString& value );
    //@}

signals:
    //! @name Signals
    //@{
    void DirectionChanged( const geometry::Vector3f& direction );
    //@}

protected:
    //! @name Operations
    //@{
    virtual void mousePressEvent( QMouseEvent* e );
    virtual void mouseMoveEvent( QMouseEvent* e );
    virtual void paintEvent( QPaintEvent* e );
    //@}

private:
    //! @name Helpers
    //@{
    void Move( const QPoint& pos );
    //@}

private:
    //! @name Member data
    //@{
    QPoint point_;
    //@}
};

} //! namespacae gui

#endif // __DirectionWidget_h_
