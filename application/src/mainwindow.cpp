#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include "protocol.hh"
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
  ui->setupUi(this);
  initObjects();
  setupLayouts();
  initConnections();
  QSettings settings("KoNaR", "AcoustiQt");
  int saved_volume = settings.value("volumeSliderPos", 50).toInt();
  ui->verticalSlider->setValue(saved_volume);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mainPageWidget() {
    ui->stackedWidget->setCurrentWidget(ui->page);
}

void MainWindow::initObjects() {
  receiver = new UdpReceiver(Protocol::PORT, this);
  processor = new FFTProcessor(this);
}

void MainWindow::setupLayouts() {
  ui->theme_frame->setParent(this);
  ui->theme_frame->hide();
}

void MainWindow::initConnections() {
  QObject::connect(ui->stackedWidget, &QStackedWidget::currentChanged,
                   ui->controlStackedWidget, &QStackedWidget::setCurrentIndex);
  QObject::connect(ui->btn_spec, &QPushButton::clicked,
                   this, &MainWindow::spectrogramPageWidget);
  QObject::connect(ui->btn_return, &QPushButton::clicked, this, &MainWindow::mainPageWidget);
  QObject::connect(ui->verticalSlider, &QSlider::valueChanged,
                  ui->spinBox, &QSpinBox::setValue);
  QObject::connect(ui->spinBox, &QSpinBox::valueChanged, ui->verticalSlider,
                   &QSlider::setValue);
  QObject::connect(receiver, &UdpReceiver::audioDataReceived, processor,
                   &FFTProcessor::handleRawAudio);
  QObject::connect(ui->verticalSlider, &QSlider::valueChanged,
                     this, &MainWindow::onVolumeSliderChanged);
  QObject::connect(processor, &FFTProcessor::spectrumReady, 
                   ui->spectrum_widget, &SpectrumVisualizer::updateSpectrum);
  QObject::connect(processor, &FFTProcessor::spectrumReady,
                   ui->spectrogram_widget, &SpectrogramVisualizer::addFFTLine);
}

void MainWindow::spectrogramPageWidget() {
    ui->stackedWidget->setCurrentWidget(ui->page_2);
}

void MainWindow::on_btn_theme_clicked()
{
  updateThemeFrameSize();
  ui->theme_frame->raise();
  ui->theme_frame->show();
}

void MainWindow::on_btn_cancel_clicked()
{
  ui->theme_frame->hide();
}

void MainWindow::resizeEvent(QResizeEvent *event) {
  QMainWindow::resizeEvent(event);

  if(ui->theme_frame && !ui->theme_frame->isHidden()) {
    updateThemeFrameSize();
    ui->theme_frame->raise();
  }
}

void MainWindow::onVolumeSliderChanged(int value) {
  double gain = 1.0;
  if (value > 0)
    gain = 1.0 * std::pow(100.0 / 1.0, static_cast<double>(value) / 100.0);
  else
    gain = 0.0;

  processor->setVolumeGain(gain);
  ui->spinBox->setValue(value);
}

void MainWindow::closeEvent(QCloseEvent *event) {
  QSettings settings("KoNaR", "AcoustiQt");
  settings.setValue("volumeSliderPos", ui->verticalSlider->value());
  QMainWindow::closeEvent(event);
}

void MainWindow::updateThemeFrameSize() {
  if (ui->theme_frame) {
    int new_width = this->width() * 0.6;
    int new_height = this->height() * 0.6;

    new_width = qMax(new_width, 300);
    new_height = qMax(new_height, 200);

    ui->theme_frame->resize(new_width, new_height);
    int x = (this->width() - ui->theme_frame->width()) / 2;
    int y = (this->height() - ui->theme_frame->height()) / 2;

    ui->theme_frame->move(x, y);
  }
}
