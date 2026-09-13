#include <SmartHomeDashboard.hpp>
#include <QVBoxLayout>

SmartHomeDashboard::SmartHomeDashboard(QWidget* parent)
    : QWidget(parent), centralLabel(new QLabel(this))
{
    setWindowState(Qt::WindowMaximized);
    setWindowTitle("Smart Home Dashboard");

    centralLabel->setText("Welcome to Smart Home Dashboard");
    centralLabel->setAlignment(Qt::AlignCenter);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addStretch();
    layout->addWidget(centralLabel, 0, Qt::AlignCenter);
    layout->addStretch();
    setLayout(layout);
}

void SmartHomeDashboard::setLabelText(const std::string &text)
{
    centralLabel->setText(text.c_str());
}