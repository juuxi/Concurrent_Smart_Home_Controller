#pragma once

#include <QWidget>
#include <QLabel>

class SmartHomeDashboard: public QWidget
{
    Q_OBJECT

public:
    SmartHomeDashboard(QWidget *parent = nullptr);
    void setLabelText(const std::string &text);

private:
    QLabel *centralLabel;
};