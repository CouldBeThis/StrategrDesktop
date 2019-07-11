﻿#ifndef NAVSCREEN_H
#define NAVSCREEN_H

#include <QLabel>
#include <QPushButton>
#include <QWidget>

class Navbar : public QWidget {
  Q_OBJECT

  QString _title;

  QPushButton *_leftButton;
  QPushButton *_rightButton;
  QLabel *_titleLabel;

  QWidget *navbar;
  QWidget *contentWidget;

  void paintEvent(QPaintEvent *) override;

  void setupWidgets();
  void setupLayout();
  void setupTitleLabel();
  void setupLeftButton();
  void setupRightButton();

  void addWidgetsToLayout();

public:
  explicit Navbar(QWidget *parent = nullptr);

  QString title() const;
  void setTitle(const QString &title);

  template <typename Method>
  void setLeftButton(
      const QString &text,
      const typename QtPrivate::FunctionPointer<Method>::Object *receiver,
      Method slot) {
    _leftButton->setText(text);
    QObject::connect(_leftButton, &QPushButton::clicked, receiver, slot);
  }

  template <typename Method>
  void setRightButton(
      const QString &text,
      const typename QtPrivate::FunctionPointer<Method>::Object *receiver,
      Method slot) {
    _rightButton->setText(text);
    QObject::connect(_rightButton, &QPushButton::clicked, receiver, slot);
  }

  QPushButton *leftButton() const;
  QPushButton *rightButton() const;
  QLabel *titleLabel() const;
};

#endif // NAVSCREEN_H