#ifndef CAROUSE_IMAGE_WINDOW
#define CAROUSE_IMAGE_WINDOW

#include <QWidget>
#include <QScrollArea>
#include <QTimer>
#include <QPropertyAnimation>
#include <QPushButton>

class CarouselImageWindow : public QWidget
{
    Q_OBJECT

public:
    CarouselImageWindow(QWidget *parent = NULL);
    ~CarouselImageWindow();

    // ����ͼƬ�б�;
    void setImageList(QStringList imageFileNameList);
    // ���ͼƬ;
    void addImage(QString imageFileName);

    // ���ð�ť�Ƿ�ɼ�;
    void setControlButtonVisible(bool isVisible);
    // ���ð�ť����ɫ;
    void setControlButtonColor(QColor buttonColor);

    // �����Ƿ���ʾ�߿�;
    void setBorderVisible(bool isVisible);
    // ���ñ߿���ɫ;
    void setBorderColor(QColor borderColor);
    // ���ñ߿���;
    void setBorderWidth(int borderWidth);
    // ���ñ߿򻡶�;
    void setBorderRadian(int borderRadian);

    // ����ͼƬ�л�ʱ��;
    void setImageChangeDuration(int duration);

    // ��ʼ����;
    // ȫ���������ý���֮���ٵ���startPlay();
    void startPlay();

signals:
    // ͼƬ����ź�;
    void clicked();

private:
    // ��ʼ��������;
    void initAnimation();
    // ��ʼ��ͼƬ�л���ť;
    void initChangeImageButton();
    // ��ͼ�¼�;
    void paintEvent(QPaintEvent *event);

    // ������¼�;
    void mousePressEvent(QMouseEvent* event);

    public slots:
    // ͼƬ�л�ʱ��;
    void onImageChangeTimeout();

    // ͼƬ�л���ť���;
    void onImageSwitchButtonClicked(int buttonId);

private:
    // ������ͼƬ�л�����Ч����Ŀǰ��͸������Ϊ�л�Ч��;
    QScrollArea * m_imagePlayWidget;
    // ͼƬ�б�;
    QList<QString> m_imageFileNameList;

    // ͼƬ�л�ʱ��;
    QTimer m_imageChangeTimer;
    // ͼƬ�л�ʱ��;
    int m_imageChangeDuration;

    // ��ǰ��ʾͼƬindex;
    int m_currentDrawImageIndx;

    // �л�ͼƬ;
    QPixmap m_currentPixmap;
    QPixmap m_nextPixmap;

    // ͼƬ�л�������;
    QPropertyAnimation* m_opacityAnimation;

    // ��ť�б�;
    QList<QPushButton*> m_pButtonChangeImageList;
    QWidget* m_buttonBackWidget;
    QColor m_buttonBackColor;

    // �߿�����;
    QColor m_borderColor;
    bool m_isShowBorder;
    int m_borderWidth;
    int m_borderRadian;
};

#endif