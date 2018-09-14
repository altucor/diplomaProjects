#ifndef FORM_FFT_H
#define FORM_FFT_H

#include <QWidget>
#include <vector>

#include "qcustomplot.h"

namespace Ui {
class form_fft;
}

class form_fft : public QWidget
{
    Q_OBJECT

public:
    explicit form_fft(QWidget *parent = 0);
    ~form_fft();
    void setPlot1Data(QVector<qint32> &data);
    void setPlot2Data(QVector<qint32> &data);
    void updatePlot1(quint64 position);
    void updatePlot2(qint64 position);
    void FFTAnalysis(std::vector<double> AVal, std::vector<double> &FTvl, int Nvl, int Nft);

private slots:
    void on_comboBox_signal_slsector_1_currentIndexChanged(int index);

    void on_comboBox_signal_slsector_2_currentIndexChanged(int index);

signals:
    void view1Changed(int);
    void view2Changed(int);

private:
    Ui::form_fft *ui;
    QVector<qint32> m_view1Data;
    QVector<qint32> m_view2Data;
};

#endif // FORM_FFT_H
