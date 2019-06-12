#include "activitygroupwidget.h"
#include "stacklayout.h"
#include "utils.h"
#include <QDebug>
#include <QLayout>
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>

ActivityGroupWidget::ActivityGroupWidget(QWidget *parent) : QWidget(parent) {
  setAttribute(Qt::WA_TransparentForMouseEvents);
  mainWidget = new QWidget(this);

  //    label = new QLabel();
  //    label->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
  //    label->setMaximumWidth(30);

  titleLabel = new QLabel();
  titleLabel->setAlignment(Qt::AlignCenter);
  titleLabel->setStyleSheet("font-weight: bold;");

  auto *mainLayout = new QHBoxLayout();
  mainLayout->setContentsMargins(0, 0, 0, 0);
  mainLayout->setSpacing(0);

  mainWidget->setLayout(mainLayout);
  //    mainWidget->layout()->addWidget(label);
  mainWidget->layout()->addWidget(titleLabel);

  selectionWidget = new QWidget(this);
  selectionWidget->hide();

  setLayout(new StackLayout());

  layout()->addWidget(mainWidget);
  layout()->addWidget(selectionWidget);

  updateStyleSheet();
}

void ActivityGroupWidget::paintEvent(QPaintEvent *event) {
  QStyleOption opt;
  opt.init(this);
  QPainter painter(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

  //  // Workaround for double rendering under scrollbar
  //  if (event->rect().width() < 30) {
  //    return;
  //  }

  //  auto height =
  //      event->rect().height() > slotHeight ? event->rect().height() :
  //      slotHeight;
  //  auto y = event->rect().y() < geometry().height() - slotHeight
  //               ? event->rect().y()
  //               : geometry().height() - slotHeight;
  //  auto textRect = QRect(0, y, geometry().width(), height);

  //  painter.drawText(textRect, Qt::AlignCenter,
  //                   activity() ? QString::fromStdString(activity()->name) :
  //                   "");
}

void ActivityGroupWidget::resizeEvent(QResizeEvent *event) {
  QWidget::resizeEvent(event);
  for (auto selectionIndex : _selectionSlots.keys()) {
    auto *selectionSlot = _selectionSlots[selectionIndex];
    selectionSlot->setGeometry(
        QRect(0, selectionIndex * slotHeight, geometry().width(), slotHeight));
  }

  update();
}

void ActivityGroupWidget::updateStyleSheet() {
  auto leftBorderStyle =
      activity() ? "border-left: 3px solid " +
                       QString::fromStdString(activity()->color) + ";"
                 : "";

  if (isSelected()) {
    setStyleSheet("ActivityGroupWidget { "
                  "background-color: #efefef;" +
                  leftBorderStyle +
                  "border-bottom: 1px solid #ccc;"
                  "}");
  } else {
    if (activity()) {
      setStyleSheet("ActivityGroupWidget { "
                    "background-color: white;" +
                    leftBorderStyle +
                    "border-bottom: 1px solid #ccc;"
                    "}");
    } else {
      setStyleSheet("ActivityGroupWidget { "
                    "background-color: #dedede;"
                    "border-bottom: 1px solid #ccc;"
                    "}");
    }
  }
}

void ActivityGroupWidget::setSlotHeight(int value) { slotHeight = value; }

int ActivityGroupWidget::slotDuration() const { return _slotDuration; }

void ActivityGroupWidget::setSlotDuration(int slotDuration) {
  _slotDuration = slotDuration;
  updateUI();
}

QMap<int, QWidget *> ActivityGroupWidget::selectionSlots() const {
  return _selectionSlots;
}

bool ActivityGroupWidget::isSelected() const { return _isSelected; }

void ActivityGroupWidget::setIsSelected(bool isSelected) {
  _isSelected = isSelected;

  updateStyleSheet();
}

void ActivityGroupWidget::setHeight(int height) { setFixedHeight(height); }

void ActivityGroupWidget::updateUI() {
  if (activity()) {
    auto name = QString::fromStdString(activity()->name);
    auto color = QString::fromStdString(activity()->color);

    titleLabel->setText("<font color=\"" + color + "\"><b>" + name +
                        "</b></font> <font color=\"#888\">" +
                        timeStringForMins(length() * slotDuration()) +
                        "</font>");
    //    titleLabel->setStyleSheet("color: " + color);

  } else {
    titleLabel->setText("");
  }

  update();
  updateStyleSheet();
}

void ActivityGroupWidget::selectSlotAtIndex(int slotIndex) {
  //    selectionWidget->repaint();
  if (!_selectionSlots.contains(slotIndex)) {
    auto *selectionSlot = new QWidget(selectionWidget);
    selectionSlot->setFixedHeight(slotHeight);
    selectionSlot->setStyleSheet("background-color: rgba(255, 255, 0, 0.5)");
    selectionSlot->setGeometry(
        QRect(0, slotIndex * slotHeight, geometry().width(), slotHeight));
    selectionSlot->show();

    _selectionSlots.insert(slotIndex, selectionSlot);
  } else {
    deselectSlotAtIndex(slotIndex);
  }

  if (selectionWidget->isHidden()) {
    selectionWidget->show();
  }
}

void ActivityGroupWidget::deselectSlotAtIndex(int slotIndex) {
  if (_selectionSlots[slotIndex] != nullptr) {
    delete _selectionSlots[slotIndex];
  }

  if (_selectionSlots.contains(slotIndex)) {
    _selectionSlots.remove(slotIndex);
  }

  if (!_selectionSlots.count()) {
    selectionWidget->hide();
  }
}

void ActivityGroupWidget::deselectAllSlots() {
  for (auto selectionIndex : _selectionSlots.keys()) {
    deselectSlotAtIndex(selectionIndex);
  }
  selectionWidget->hide();
}

bool ActivityGroupWidget::hasSelection() { return _selectionSlots.count() > 0; }

int ActivityGroupWidget::length() const { return _length; }

void ActivityGroupWidget::setLength(unsigned int length) {
  _length = static_cast<int>(length);
  updateUI();
}

std::optional<Activity> ActivityGroupWidget::activity() const {
  return _activity;
}

void ActivityGroupWidget::setActivity(std::optional<Activity> activity) {
  _activity = activity;
  updateUI();
}

int ActivityGroupWidget::number() const { return _number; }

void ActivityGroupWidget::setNumber(int number) {
  _number = number;
  //    label->setText(QString::number(number));
}
