// Copyright (c) 2011-2020 The Revolt Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REVOLT_QT_TRAFFICGRAPHWIDGET_H
#define REVOLT_QT_TRAFFICGRAPHWIDGET_H

#include <QWidget>
#include <QQueue>

#include <chrono>

class ClientModel;

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QTimer;
QT_END_NAMESPACE

class TrafficGraphWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TrafficGraphWidget(QWidget *parent = nullptr);
    void setClientModel(ClientModel *model);
    std::chrono::minutes getGraphRange() const;

protected:
    void paintEvent(QPaintEvent *) override;

public Q_SLOTS:
    void updateRates();
    void setGraphRange(std::chrono::minutes new_range);
    void clear();

private:
    void paintPath(QPainterPath &path, QQueue<float> &samples);

    QTimer *timer;
    float fMax;
    std::chrono::minutes m_range{0};
    QQueue<float> vSamplesIn;
    QQueue<float> vSamplesOut;
    quint64 nLastBytesIn;
    quint64 nLastBytesOut;
    ClientModel *clientModel;
};

#endif // REVOLT_QT_TRAFFICGRAPHWIDGET_H
