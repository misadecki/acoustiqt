#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include "protocol.hh"
#include <QSettings>
#include <QTimer>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
  ui->setupUi(this);
  connectThemes();
  initObjects();
  setupLayouts();
  initConnections();
  initStatsConnections();
  initLanguages();
  setPreviousSettings();
  updateLanguages();
  alignObjects();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mainPageWidget() {
    ui->stackedWidget->setCurrentWidget(ui->page);
}

void MainWindow::connectThemes() {
  connect(ui->btn_dark, &QPushButton::clicked, this, [this]() {
    loadTheme("dark");
  });

  connect(ui->btn_light, &QPushButton::clicked, this, [this]() {
    loadTheme("light");
  });

  connect(ui->btn_intensive, &QPushButton::clicked, this, [this]() {
    loadTheme("intense");
  });
}

void MainWindow::connectAxisUnits() {
  connect(ui->unitBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
          this, [this](int index) {
            AxisFormat format = static_cast<AxisFormat>(index);
            ui->spectrum_widget->setFrequencyFormat(format);

            QSettings settings(org, application);
            settings.setValue("FreqFormat", index);
  });
}

void MainWindow::setPreviousSettings() {
  QSettings settings(org, application);
  int saved_volume = settings.value("volumeSliderPos", 50).toInt();
  QString saved_lang = settings.value("SavedLanguage", "pl").toString();
  int saved_freq_format = settings.value("FreqFormat", 0).toInt();
  int saved_idx = ui->lang_box->findData(saved_lang);
  if (saved_idx != -1)
    ui->lang_box->setCurrentIndex(saved_idx);
  ui->verticalSlider->setValue(saved_volume);
  ui->unitBox->setCurrentIndex(saved_freq_format);

  AxisFormat init_format = static_cast<AxisFormat>(saved_freq_format);
  ui->spectrum_widget->setFrequencyFormat(init_format);

  QString saved_theme = settings.value("SavedTheme", "light").toString();
  loadTheme(saved_theme);
}

void MainWindow::initLanguages() {
  ui->lang_box->setItemData(0, "pl");
  ui->lang_box->setItemData(1, "en");
}

void MainWindow::updateLanguages() {
  connect(ui->lang_box, QOverload<int>::of(&QComboBox::currentIndexChanged),
          this, &MainWindow::changeLanguage);
  changeLanguage(ui->lang_box->currentIndex());
}

void MainWindow::alignObjects() {
  ui->lang_box->setEditable(true);
  ui->lang_box->lineEdit()->setAlignment(Qt::AlignCenter);
  ui->lang_box->lineEdit()->setReadOnly(true);
  ui->lang_box->lineEdit()->installEventFilter(this);
  ui->lang_box->setItemData(0, Qt::AlignCenter, Qt::TextAlignmentRole);
  ui->lang_box->setItemData(1, Qt::AlignCenter, Qt::TextAlignmentRole);
  ui->lang_box->lineEdit()->setCursor(Qt::PointingHandCursor);
  ui->unitBox->setItemData(0, Qt::AlignCenter, Qt::TextAlignmentRole);
  ui->unitBox->setItemData(1, Qt::AlignCenter, Qt::TextAlignmentRole);
  ui->unitBox->setItemData(2, Qt::AlignCenter, Qt::TextAlignmentRole);
  ui->lang_flag->setAlignment(Qt::AlignCenter);
}

void MainWindow::initObjects() {
  receiver = new UdpReceiver(Protocol::PORT, this);
  processor = new FFTProcessor(this);
  timer = new QTimer(this);
  db_client = new InfluxClient(this);
}

void MainWindow::setupLayouts() {
  ui->theme_frame->setParent(this);
  ui->theme_frame->hide();
}

void MainWindow::initStatsConnections() {
  QObject::connect(timer, &QTimer::timeout, this, &MainWindow::onTimerTimeout);
  QObject::connect(processor, &FFTProcessor::statsReady, this,
                   &MainWindow::updateStats);

  timer->start(3000);
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
  connectAxisUnits();
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
  QSettings settings(org, application);
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

void MainWindow::on_btn_stop_clicked() {
  if (is_running) {
    QObject::disconnect(receiver, &UdpReceiver::audioDataReceived, processor,
                        &FFTProcessor::handleRawAudio);
    ui->btn_stop->setText("Resume");
    is_running = false;
  } else {
    QObject::connect(receiver, &UdpReceiver::audioDataReceived, processor,
                        &FFTProcessor::handleRawAudio);
    ui->btn_stop->setText("Stop");
    is_running = true;
  }
}

void MainWindow::onTimerTimeout() {
  ui->ledit_peak->setText(QString::number(latest_stats.peak, 'f', 4));
  ui->ledit_zcr->setText(QString::number(latest_stats.zcr));
  ui->ledit_rms->setText(QString::number(latest_stats.rms, 'f', 4));
  double freq = latest_stats.dominant_freq;
  QString unit = ui->unitBox->currentText();

  if (unit == "kHz")
    freq = freq / 1000.0;

  ui->ledit_fd->setText(QString::number(freq, 'f', 2));
  db_client->sendStats(latest_stats);
}

void MainWindow::loadTheme(const QString &themeName) {
  QFile file(QString(":/themes/%1.qss").arg(themeName));

  if (!file.open(QFile::ReadOnly | QFile::Text)) {
    qWarning() << "Can't open theme file: " << QString(":/themes/%1.qss").arg(themeName);
    return;
  }

  QTextStream stream(&file);
  QString css = stream.readAll();

  qApp->setStyleSheet(css);
  file.close();

  QSettings settings(org, application);
  settings.setValue("SavedTheme", themeName);
}

void MainWindow::changeLanguage(int index) {
  QString langCode = ui->lang_box->itemData(index).toString();

  if (app_translator.load(":/i18n/app_" + langCode + ".qm")) {
    qApp->installTranslator(&app_translator);
    ui->retranslateUi(this);
  }

  QString flagPath = QString(":/flags/imgs/%1.png").arg(langCode);
  QPixmap flagImg(flagPath);

  if (!flagImg.isNull()) {
    ui->lang_flag->setPixmap(flagImg.scaled(56, 40, Qt::KeepAspectRatio,
                                            Qt::SmoothTransformation));
  }

  QSettings settings(org, application);
  settings.setValue("SavedLanguage", langCode);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
  if (watched == ui->lang_box->lineEdit() && event->type() ==
    QEvent::MouseButtonRelease) {
    ui->lang_box->showPopup();
    return true;
  }

  return QMainWindow::eventFilter(watched, event);
}
