#include "form_fft.h"
#include "ui_form_fft.h"

form_fft::form_fft(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::form_fft)
{
    ui->setupUi(this);
    this->setWindowTitle("FFT");

    ui->comboBox_signal_slsector_1->addItem("Accelerometer X");
    ui->comboBox_signal_slsector_1->addItem("Accelerometer Y");
    ui->comboBox_signal_slsector_1->addItem("Accelerometer Z");
    ui->comboBox_signal_slsector_1->addItem("Gyroscope X");
    ui->comboBox_signal_slsector_1->addItem("Gyroscope Y");
    ui->comboBox_signal_slsector_1->addItem("Gyroscope Z");

    ui->comboBox_signal_slsector_2->addItem("Accelerometer X");
    ui->comboBox_signal_slsector_2->addItem("Accelerometer Y");
    ui->comboBox_signal_slsector_2->addItem("Accelerometer Z");
    ui->comboBox_signal_slsector_2->addItem("Gyroscope X");
    ui->comboBox_signal_slsector_2->addItem("Gyroscope Y");
    ui->comboBox_signal_slsector_2->addItem("Gyroscope Z");
}

form_fft::~form_fft()
{
    delete ui;
}

void form_fft::setPlot1Data(QVector<qint32> &data)
{

}

void form_fft::setPlot2Data(QVector<qint32> &data)
{

}

void form_fft::updatePlot1(quint64 position)
{

}

void form_fft::updatePlot2(qint64 position)
{

}

void form_fft::FFTAnalysis(std::vector<double> AVal, std::vector<double> &FTvl, int Nvl, int Nft)
{
    const double TwoPi = 6.283185307179586;
    int i, j, n, m, Mmax, Istp;
    double Tmpr, Tmpi, Wtmp, Theta;
    double Wpr, Wpi, Wr, Wi;
    double *Tmvl;

    n = Nvl * 2; Tmvl = new double[n];

    for (i = 0; i < n; i+=2) {
        Tmvl[i] = 0;
        Tmvl[i+1] = AVal[i/2];
    }

    i = 1; j = 1;
    while (i < n) {
        if (j > i) {
            Tmpr = Tmvl[i]; Tmvl[i] = Tmvl[j]; Tmvl[j] = Tmpr;
            Tmpr = Tmvl[i+1]; Tmvl[i+1] = Tmvl[j+1]; Tmvl[j+1] = Tmpr;
        }
        i = i + 2; m = Nvl;
        while ((m >= 2) && (j > m)) {
            j = j - m; m = m >> 1;
        }
        j = j + m;
    }

    Mmax = 2;
    while (n > Mmax) {
        Theta = -TwoPi / Mmax; Wpi = sin(Theta);
        Wtmp = sin(Theta / 2); Wpr = Wtmp * Wtmp * 2;
        Istp = Mmax * 2; Wr = 1; Wi = 0; m = 1;

        while (m < Mmax) {
            i = m; m = m + 2; Tmpr = Wr; Tmpi = Wi;
            Wr = Wr - Tmpr * Wpr - Tmpi * Wpi;
            Wi = Wi + Tmpr * Wpi - Tmpi * Wpr;

            while (i < n) {
                j = i + Mmax;
                Tmpr = Wr * Tmvl[j] - Wi * Tmvl[j-1];
                Tmpi = Wi * Tmvl[j] + Wr * Tmvl[j-1];

                Tmvl[j] = Tmvl[i] - Tmpr; Tmvl[j-1] = Tmvl[i-1] - Tmpi;
                Tmvl[i] = Tmvl[i] + Tmpr; Tmvl[i-1] = Tmvl[i-1] + Tmpi;
                i = i + Istp;
            }
        }
        Mmax = Istp;
    }

    for (i = 0; i < Nft; i++) {
        j = i * 2; FTvl[i] = 2*sqrt(pow(Tmvl[j],2) + pow(Tmvl[j+1],2))/Nvl;
    }

  delete []Tmvl;
}

void form_fft::on_comboBox_signal_slsector_1_currentIndexChanged(int index)
{

}

void form_fft::on_comboBox_signal_slsector_2_currentIndexChanged(int index)
{

}
