// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __gui_ExpandableGroupBox_h_
#define __gui_ExpandableGroupBox_h_

namespace gui
{

// =============================================================================
/** @class  ExpandableGroupBox
    @brief  ExpandableGroupBox
*/
// Created: ABR 2012-06-21
// =============================================================================
class ExpandableGroupBox : public QFrame
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit ExpandableGroupBox( QWidget* parent = 0, const QString& title = "", Qt::Orientation orientation = Qt::Vertical );
    virtual ~ExpandableGroupBox();
    //@}

    //! @name Operations
    //@{
    void SetTitle( const QString& title );
    const QString& Title() const;

    void AddComponent( QWidget* component );
    QList< QWidget* > Components() const;

    void SetOrientation( Qt::Orientation orientation );
    Qt::Orientation Orientation() const;

    void Collapse();
    void Expand();
    void SetExpanded( bool expanded );
    bool IsExpanded() const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnCollapseClicked();
    //@}

private:
    //! @name Member data
    //@{
    QString           title_;
    QLabel*           titleLabel_;
    QFrame*           titleWidget_;
    QPushButton*      expandButton_;
    QFrame*           componentsWidget_;
    QList< QWidget* > components_;
    Qt::Orientation   orientation_;
    QPoint            componentsOrigin_;
    QIcon             expandIcon_;
    QIcon             collapseIcon_;
    //@}
};

} //! namespace gui

#endif // __gui_ExpandableGroupBox_h_
