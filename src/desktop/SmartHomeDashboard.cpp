#include <SmartHomeDashboard.hpp>
#include <QVBoxLayout>

SmartHomeDashboard::SmartHomeDashboard(QWidget* parent)
    : QWidget(parent)
{
    setWindowState(Qt::WindowMaximized);
    setWindowTitle("Smart Home Dashboard");

    labelContainer = new QWidget(this);
    labelLayout = new QHBoxLayout(labelContainer);
    labelLayout->setContentsMargins(10, 10, 10, 10);
    labelLayout->setSpacing(10);
    labelLayout->addStretch();

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(labelContainer);
    layout->addStretch();
    setLayout(layout);
}

void SmartHomeDashboard::addLabel(const std::string& text)
{
    QLabel *newLabel = new QLabel(QString::fromStdString(text), labelContainer);
    newLabel->setStyleSheet("background-color: #f0f0f0; padding: 8px; border-radius: 4px; border: 1px solid #ddd;");
    
    labels.push_back(newLabel);
    
    labelLayout->insertWidget(labelLayout->count() - 1, newLabel);
}

QLabel* SmartHomeDashboard::getLabel(int index)
{
    if (index >= 0 && index < static_cast<int>(labels.size()))
        return labels[index];
    return nullptr;
}

void SmartHomeDashboard::setLabelText(int index, const std::string& text)
{
    QLabel* label = getLabel(index);
    if (label)
        label->setText(QString::fromStdString(text));
}