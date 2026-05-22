/**
 * @file mainwindow.hh
 * @author Michał Sadecki (michal.sadecki@proton.me)
 * @brief Plik ten zawiera funkcje związane z głównym oknem aplikacji. 
 * @version 0.3
 * @date 2026-04-22
 *
 * @copyright Copyright (c) 2026 Michał Sadecki
 */

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QDialog>
#include <QStackedWidget>
#include <QLineEdit> 
#include <QPushButton>
#include <QLabel>
#include <QResizeEvent>
#include "udp_receiver.hh"
#include "spectrum_visualizer.hh"
#include "fft_processor.hh"
#include "spectrogram_visualizer.hh"

QT_BEGIN_NAMESPACE
namespace Ui {
  class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Ui::MainWindow *ui;
  UdpReceiver *receiver;
  FFTProcessor *processor;
  SpectrumVisualizer *spectrum_visualizer;
  SpectrogramVisualizer *spectrogram_visualizer;

  /**
  * @brief Alokuje pamięć dla obiektów zawartych w klasie. 
  */
  void initObjects();

  /**
  * @brief Ustawia odpowiednie zależności pomiędzy widokami.
  */
  void setupLayouts();

  /**
  * @brief Realizuje połączenia pomiędzy sygnałami i slotami.
  */
  void initConnections();
  void updateThemeFrameSize();
  Q_OBJECT

private slots:
  void onVolumeSliderChanged(int value);
public:
  /**
   * @brief Konstruktor inicjalizujący główne okno aplikacji. Ustawia sygnały i sloty.
   *
   * @param[in] parent - wskaźnik na rodzica MainWindow.
   */
  explicit MainWindow(QWidget *parent = nullptr);

  /**
   * @brief Domyślny destruktor głównego okna aplkacji.
   */
  ~MainWindow() override;
protected:
  /**
  * @brief Przelicza rozmiar okna w zależności od skali ustalonej przez
  * użytkownika.
  *
  * @param[in] event - Wskaźnik na zdarzenie.
  */
  void resizeEvent(QResizeEvent *event) override;
  void closeEvent(QCloseEvent *event) override;
public slots:
  /**
   * @brief Obsługuje główny widżet wizualizacji widma.
   */
  void mainPageWidget();

  /**
  * @brief Obsługuje widżet spektrogramu.
  */
  void spectrogramPageWidget();
private slots:
  /**
  * @brief Obsługuje wciśnięcie przycisku "Theme".
  */
  void on_btn_theme_clicked();

  /**
  * @brief Obsługuje wciśnięcie przycisku "Cancel".
  */
  void on_btn_cancel_clicked();
};
#endif // MAINWINDOW_HH
