#include "gui/QConditionalControl.h"
#include <QEvent>
#include <iostream>


QWidgetList QConditionalControl::getWidgets(QLayout *layout) {
    QWidgetList widgets;
    for (int i = 0; i < layout->count(); i++) {
        QWidget *w = layout->itemAt(i)->widget();
        if (w != nullptr) {
            widgets.push_back(w);
        } else {
            QLayout *l = layout->itemAt(i)->layout();
            if (l != nullptr) {
                QWidgetList tmp_wl = getWidgets(l);
                widgets.append(tmp_wl);
            }
        }
    }
    return widgets;
}



void QConditionalControl::setEnabledWidgetsInLayout(QLayout *layout, bool enable) {
    for (int i = 0; i < layout->count(); ++i) {
        QWidget *w = layout->parentWidget();
        auto widgets = w->findChildren<QWidget *>(QString(), Qt::FindDirectChildrenOnly);
        for (auto &el : widgets) {
            if (el != mCheckBox) {
                el->setEnabled(enable);
            }
        }
    }
}

void QConditionalControl::setEventFilterForWidgets(QLayout *layout) {
    for (int i = 0; i < layout->count(); ++i) {
        QWidget *w = layout->parentWidget();
        auto widgets = w->findChildren<QWidget *>(QString(), Qt::FindDirectChildrenOnly);
        for (auto &el : widgets) {
            if (el != mCheckBox) {
                el->installEventFilter(this);
            }
        }
    }
}


QConditionalControl::QConditionalControl() {
    mCheckBox->setCheckState(Qt::CheckState::Unchecked);
    mLayout->addWidget(mCheckBox);
    setLayout(mLayout);
    QObject::connect(mCheckBox, &QCheckBox::stateChanged, this, &QConditionalControl::stateUpdate);
}

void QConditionalControl::addLayout(QLayout *layout, size_t stretch) {
    mLayout->addLayout(layout, stretch);
    QWidgetList widgets = getWidgets(layout);
    for (auto &el : widgets) {
        el->installEventFilter(this);
        mExternWidgets.push_back(Controller(el));
    }
}

bool QConditionalControl::isChecked() {
    return mCheckBox->isChecked();
}

void QConditionalControl::addWidget(QWidget *widget, size_t stretch) {
    mLayout->addWidget(widget, stretch);
    mExternWidgets.push_back(Controller(widget));
    widget->installEventFilter(this);
    if (!isChecked()) {
        widget->setEnabled(false);
    }
}

void QConditionalControl::stateUpdate(int state) {
    switch (state) {
        case Qt::CheckState::Unchecked: {
            for (auto &el : mExternWidgets) {
                el.setEnabled(false);
            }
            break;
        }
        case Qt::CheckState::Checked: {
            for (auto &el : mExternWidgets) {
                if (el.mIsEnabled) {
                    el.setEnabled(true);
                }
            }
            break;
        }
        default:
            break;
    }
}

void QConditionalControl::setDisableChangingState(bool disableChangingState) {
    mCheckBox->setDisabled(disableChangingState);
}

bool QConditionalControl::isDisableChangingState() {
    return !mCheckBox->isEnabled();
}
// TODO(aleksey) Отстутсвует возможность отключить внутренний виджет когода фалг опущен. Таким образом
//  Если флаг опущен, извне виджет отключается, а затем флаг поднимается, то виджет, который должен быть отключен
//  будет включенным.
bool QConditionalControl::eventFilter(QObject *object, QEvent *event) {
    if (event->type() == QEvent::Type::EnabledChange) {
        auto *widget = dynamic_cast<QWidget*>(object);
        Controller *c = findController(widget);
        if (!c->mPrivateChange) {
            c->mIsEnabled = c->mWidget->isEnabled();
            if (c->mIsEnabled && !isChecked()) {
                c->setEnabled(false);
            }
        }
    }
    return false;
}

QConditionalControl::Controller* QConditionalControl::findController(QWidget *widget) {
    for (auto &el : mExternWidgets) {
        if (el.mWidget == widget) {
            return &el;
        }
    }
    return nullptr;
}

QConditionalControl::Controller::Controller(QWidget *widget) : mWidget(widget) {
    mIsEnabled = mWidget->isEnabled();
}

void QConditionalControl::Controller::setEnabled(bool enable) {
    mPrivateChange = true;
    mWidget->setEnabled(enable);
    mPrivateChange = false;
}
