#pragma once

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <vector>

class SmartHomeDashboard: public QWidget
{
    Q_OBJECT

public:
    SmartHomeDashboard(QWidget *parent = nullptr);
    void setLabelText(int index, const std::string& text);
    
    void addLabel(const std::string& text);
    QLabel* getLabel(int index);

private:
    QWidget *labelContainer;
    QHBoxLayout *labelLayout;
    std::vector<QLabel*> labels;
};