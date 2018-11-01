#include "CarouselImageWindow.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QDebug>
#include <QButtonGroup>
#include <QGraphicsDropShadowEffect>

CarouselImageWindow::CarouselImageWindow(QWidget *parent)
    : QWidget(parent)
    , m_currentDrawImageIndx(0)
    , m_isShowBorder(true)
    , m_borderColor(Qt::red)
    , m_buttonBackColor(Qt::white)
    , m_borderWidth(5)
    , m_borderRadian(3)
    , m_imageChangeDuration(2000)
{
    initAnimation();

    // ��ťBackWidget;
    m_buttonBackWidget = new QWidget;
    m_buttonBackWidget->setStyleSheet(".QWidget{background:transparent;}");

    this->setFixedSize(QSize(450, 650));
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
}

CarouselImageWindow::~CarouselImageWindow()
{

}

void CarouselImageWindow::initAnimation()
{
    // ���ImageOpacity����;
    this->setProperty("ImageOpacity", 1.0);
    // �����л���;
    m_opacityAnimation = new QPropertyAnimation(this, "ImageOpacity");
    // ����Ҫ���õĶ���ʱ��С��ͼƬ�л�ʱ��;
    m_opacityAnimation->setDuration(m_imageChangeDuration - 200);
    m_opacityAnimation->setEasingCurve(QEasingCurve::InOutQuart);
    // ����ImageOpacity����ֵ�ı仯��Χ;
    m_opacityAnimation->setStartValue(1.0);
    m_opacityAnimation->setEndValue(0.0);
    // ͸���ȱ仯��ʱ���»�ͼ;
    connect(m_opacityAnimation, SIGNAL(valueChanged(const QVariant&)), this, SLOT(update()));
    // ����ͼƬ�л�ʱ�Ӳۺ���;
    connect(&m_imageChangeTimer, SIGNAL(timeout()), this, SLOT(onImageChangeTimeout()));
}

void CarouselImageWindow::initChangeImageButton()
{
    // ע��ͼƬ���ఴť���ܷ��ò���;
    QButtonGroup* changeButtonGroup = new QButtonGroup;
    QHBoxLayout* hLayout = new QHBoxLayout(m_buttonBackWidget);
    hLayout->addStretch();
    for (int i = 0; i < m_imageFileNameList.count(); i++)
    {
        QPushButton* pButton = new QPushButton;
        pButton->setFixedSize(QSize(18, 18));
        pButton->setCheckable(true);
        pButton->setStyleSheet(QString("QPushButton{background:rgb(%1, %2, %3);border-radius:6px;margin:3px;}\
                                QPushButton:checked{border-radius:9px;margin:0px;}").arg(m_buttonBackColor.red()).arg(m_buttonBackColor.green()).arg(m_buttonBackColor.blue()));

        changeButtonGroup->addButton(pButton, i);
        m_pButtonChangeImageList.append(pButton);
        hLayout->addWidget(pButton);
    }
    hLayout->addStretch();
    hLayout->setSpacing(10);
    hLayout->setMargin(0);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch();
    mainLayout->addWidget(m_buttonBackWidget);
    mainLayout->setContentsMargins(0, 0, 0, 30);

    connect(changeButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(onImageSwitchButtonClicked(int)));
}

void CarouselImageWindow::setImageList(QStringList imageFileNameList)
{
    m_imageFileNameList = imageFileNameList;
}

void CarouselImageWindow::addImage(QString imageFileName)
{
    m_imageFileNameList.append(imageFileName);
}

void CarouselImageWindow::startPlay()
{
    // �����ͼƬ֮�󣬸���ͼƬ��������ͼƬ�л���ť;
    initChangeImageButton();
    if (m_imageFileNameList.count() == 1)
    {
        m_pButtonChangeImageList[m_currentDrawImageIndx]->setChecked(true);
    }
    else if (m_imageFileNameList.count() > 1)
    {
        m_pButtonChangeImageList[m_currentDrawImageIndx]->setChecked(true);
        m_currentPixmap = QPixmap(m_imageFileNameList.at(m_currentDrawImageIndx));
        m_imageChangeTimer.start(m_imageChangeDuration);
        update();
    }
}

void CarouselImageWindow::setControlButtonVisible(bool isVisible)
{
    m_buttonBackWidget->setVisible(isVisible);
}

void CarouselImageWindow::setControlButtonColor(QColor buttonColor)
{
    m_buttonBackColor = buttonColor;
}

void CarouselImageWindow::setBorderVisible(bool isVisible)
{
    m_isShowBorder = isVisible;
}

void CarouselImageWindow::setBorderColor(QColor borderColor)
{
    m_borderColor = borderColor;
}

void CarouselImageWindow::setBorderWidth(int borderWidth)
{
    m_borderWidth = borderWidth;
}

void CarouselImageWindow::setBorderRadian(int borderRadian)
{
    m_borderRadian = borderRadian;
}

void CarouselImageWindow::setImageChangeDuration(int duration)
{
    m_imageChangeDuration = duration;
}

void CarouselImageWindow::onImageChangeTimeout()
{
    // ����ǰ���ͼƬ;
    m_currentPixmap = QPixmap(m_imageFileNameList.at(m_currentDrawImageIndx));
    m_currentDrawImageIndx++;
    if (m_currentDrawImageIndx >= m_imageFileNameList.count())
    {
        m_currentDrawImageIndx = 0;
    }
    m_nextPixmap = QPixmap(m_imageFileNameList.at(m_currentDrawImageIndx));

    m_pButtonChangeImageList[m_currentDrawImageIndx]->setChecked(true);

    // ���������¿�ʼ; 
    m_opacityAnimation->start();
}

void CarouselImageWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QRect imageRect = this->rect();
    // �Ƿ���Ʊ߿�;
    if (m_isShowBorder)
    {
        imageRect.adjust(m_borderWidth, m_borderWidth, -m_borderWidth, -m_borderWidth);
    }

    painter.save();
    // ���ͼƬ�б�Ϊ�գ���ʾĬ��ͼƬ;
    if (m_imageFileNameList.isEmpty())
    {
        QPixmap backPixmap = QPixmap(":/Resources/CarouselImageBack.png");
        painter.drawPixmap(imageRect, backPixmap.scaled(imageRect.size()));
    }
    // ���ֻ��һ��ͼƬ;
    else if (m_imageFileNameList.count() == 1)
    {
        QPixmap backPixmap = QPixmap(m_imageFileNameList.first());
        painter.drawPixmap(imageRect, backPixmap.scaled(imageRect.size()));
    }
    // ����ͼƬ;
    else if (m_imageFileNameList.count() > 1)
    {
        float imageOpacity = this->property("ImageOpacity").toFloat();
        painter.setOpacity(1);
        painter.drawPixmap(imageRect, m_nextPixmap.scaled(imageRect.size()));
        painter.setOpacity(imageOpacity);
        painter.drawPixmap(imageRect, m_currentPixmap.scaled(imageRect.size()));
    }

    painter.restore();
    if (m_isShowBorder)
    {
        qreal adjustedValue = 1.0 * m_borderWidth / 2;
        painter.setPen(QPen(m_borderColor, m_borderWidth));
        QRectF widgetRect = this->rect();
        painter.drawRoundedRect(widgetRect.adjusted(adjustedValue, adjustedValue, -adjustedValue - 1, -adjustedValue - 1), m_borderRadian, m_borderRadian);
    }
}

void CarouselImageWindow::onImageSwitchButtonClicked(int buttonId)
{
    m_currentDrawImageIndx = buttonId - 1;
    if (m_currentDrawImageIndx == -1)
    {
        m_currentDrawImageIndx = m_imageFileNameList.count() - 1;
    }

    onImageChangeTimeout();
    m_imageChangeTimer.start(m_imageChangeDuration);
    update();
}

void CarouselImageWindow::mousePressEvent(QMouseEvent* event)
{
    // ������ԶԵ�ǰͼƬ���е��Ȼ�󴥷�ÿ��ͼƬ��Ӧ��Ч��;
    // ����web�Ϻö����Ƶĵ�����Ӧ�Ĺ��ҳ��ȹ���;
    emit clicked();
    qDebug() << m_currentDrawImageIndx;
    return __super::mousePressEvent(event);
}